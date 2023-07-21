#include "vofa.h"

//Vofa专用缓冲区
float vofa_ch_data[ch_sz]    = {1.2,2.4,3.6,4.8};                  //测试用通道数据
uint8_t custom_buf[custom_sz]  = {0};                              //下位机发送状态数据缓冲区
uint8_t cmd_rxbuf[cmd_sz] = {0};                                   //下位机接收命令缓冲区


/**********************************************************************************************************
 *  @fcn    初始化VOFA协议对象、释放VOFA对象占用的资源
 *  @para vofa_pt:vofa对象
 *              ch_data_pt:通道型数据缓冲区
 *********************************************************************************************************/
VOFA* vofa_create(void){
        VOFA* vofa_pt = (VOFA*)malloc(sizeof(VOFA));                //创建VOFA对象
                                                                    //可以通过链表来管理多个同类的对象
                                                                    //---
        return vofa_pt;
}

VOFA_STATE vofa_init(VOFA* vofa_pt,
                                            void*    ch_data_pt     ,uint32_t ch_size,\
                                            uint8_t* custom_buf_pt  ,uint32_t custom_size,\
                                            uint8_t* cmd_rxbuf_pt   ,uint32_t cmd_size,\
                                            uint8_t  VOFA_UART        ,uint8_t  BLE_UART,uint8_t SCOPE_UART)
{
        //vofa数据缓冲区
            //1.普通数据缓冲区入口(主要用于把下位机的数据发送到上位机)
        vofa_pt->ch_data_pt = ch_data_pt;
        vofa_pt->ch_size      = ch_size;
            //2.自定义型数据缓冲区入口
        vofa_pt->custom_buf_pt  = custom_buf_pt;
        vofa_pt->custom_buf_size = custom_size;
            //3.控制指令型数据缓冲区入口(主要用于接收上位机的控制指令)
        vofa_pt->cmd_rxbuf  = cmd_rxbuf_pt;
        vofa_pt->cmd_size       = cmd_size;

        //物理实现端口
            //1.vofa上位机的端口
        vofa_pt->VOFA_UART  = VOFA_UART;
            //2.蓝牙调试器的端口
        vofa_pt->BLE_UART   = BLE_UART;
            //3.虚拟示波器的端口
        vofa_pt->SCOPE_UART = SCOPE_UART;

        //协议规定的引导部分
            //1.JF、FW通用的图像数据前导帧(用户可调用image_property_set来vofa_preframe数组的前5个元素，这些元素用来描述待传输图片的信息)
        vofa_pt->vofa_preframe[5] = 0x7F800000;
        vofa_pt->vofa_preframe[6] = 0x7F800000;
            //2.JF通道型数据的固定帧尾
        vofa_pt->vofa_JF_tail[0] = 0x00;
        vofa_pt->vofa_JF_tail[1] = 0x00;
        vofa_pt->vofa_JF_tail[2] = 0x80;
        vofa_pt->vofa_JF_tail[3] = 0x7F;

        //vofa对象包含的方法
        vofa_pt->vofa_release  = vofa_release;
        vofa_pt->sendzip = vofa_sendzip;
        return VOFA_OK;
}

//反初始化
VOFA_STATE vofa_release(VOFA* vofa_pt){
    if(!vofa_pt)    free(vofa_pt);
    else    return VOFA_ERR;
    return VOFA_OK;
}



/**********************************************************************
 *  @fcn    数据发送函数(私有函数)
 *  @para vofa_pt:vofa对象
 *              vofa_frame:待传输的数据帧
 **********************************************************************/
//发送数据帧头或帧尾（引导部分）
static VOFA_STATE vofa_transmit_leader(VOFA* vofa_pt,uint8_t vofa_frame_leader){
    if(vofa_pt == NULL) return VOFA_ERR;
    if(vofa_frame_leader == JF_TAIL){
        uart_write_buffer(vofa_pt->VOFA_UART,vofa_pt->vofa_JF_tail,4);                      //向上位机发送JF通道型数据的帧尾：4byte
        return VOFA_OK;
    }
    else if(vofa_frame_leader == JF_PREFRAME){
        uart_write_buffer(vofa_pt->VOFA_UART,(uint8_t*)(vofa_pt->vofa_preframe),28);        //向上位机发送JF图像前导帧：28byte
        return VOFA_OK;
    }
    return VOFA_ERR;
}


//发送用户数据
static VOFA_STATE vofa_transmit(VOFA* vofa_pt,uint8_t vofa_frame_category){
    if(vofa_pt == NULL) return VOFA_ERR;
    if(vofa_frame_category == VOFA_CH_FRAME){
        uart_write_buffer(vofa_pt->VOFA_UART,(uint8_t*)(vofa_pt->ch_data_pt),vofa_pt->ch_size);
        return VOFA_OK;
    }
    else if(vofa_frame_category == VOFA_IMAGE_FRAME){
        uart_write_buffer(vofa_pt->VOFA_UART,(uint8_t*)(vofa_pt->image_pt),vofa_pt->vofa_preframe[2]);
        return VOFA_OK;
    }
    return VOFA_ERR;
}



/**********************************************************************
 *  @fcn    数据打包成用户指定的类型并发送
 *  @para vofa_pt:VOFA对象;
 *              vofa_protocal:数据传输协议
 *              vofa_frame_category:待传输数据的类型
 *              rawdata为原始数据
 **********************************************************************/
//下位机数据打包
VOFA_STATE vofa_sendzip(VOFA* vofa_pt,uint8_t vofa_protocol,uint8_t vofa_frame_category){
    if(vofa_pt == NULL) return VOFA_ERR;
        //JF协议//
        if(vofa_protocol == VOFA_PROTOCOL_JUSTFLOAT){
            if(vofa_frame_category == VOFA_CH_FRAME){                                        //小数据通道模式
                        if(vofa_transmit(vofa_pt,VOFA_CH_FRAME)){                            //发送用户数据(通道类型)
                                vofa_transmit_leader(vofa_pt,JF_TAIL);                       //发送帧尾
                                return VOFA_OK;
                        }
                        else return VOFA_ERR;
             }

//            else if(vofa_frame_category == VOFA_IMAGE_FRAME){                           //图像模式
//                vofa_transmit_leader(vofa_pt,JF_PREFRAME);                              //发送前导帧
//                if(vofa_transmit(vofa_pt,VOFA_IMAGE_FRAME)){                            //发送用户数据（图像类型）
//                    return VOFA_OK;
//                }
//            }
        }

        //FW协议//
        else if(vofa_protocol == VOFA_PROTOCOL_FIREWATER){
            if(vofa_frame_category == VOFA_CH_FRAME){
                uint32_t ch_size = vofa_pt->ch_size/sizeof(float);
                uint8_t i = 0;
                for(;i < ch_size;i++){
                    VOFA_Printf("%f",((float*)(vofa_pt->ch_data_pt))[i]);              //小数据通道模式（无前导帧）
                    if(i < ch_size-1){
                        VOFA_Printf(",");
                    }
                }
                VOFA_Printf("\n");                                                    //换行符（FW协议的帧尾）
                return VOFA_OK;
            }
        }
        else if(vofa_protocol == EXT_PROTOCOL_BLEDEBUGGER){
            uint16_t checksum = 0;
            uart_write_byte(vofa_pt->BLE_UART,0xA5);                                     //发送帧头
            uart_write_buffer(vofa_pt->BLE_UART,custom_buf,sizeof(custom_buf));          //发送用户数据
            for (int i = 1;i < sizeof(custom_buf)-2;i++){
                checksum += custom_buf[i];
            }
            uart_write_byte(vofa_pt->BLE_UART,(uint8_t)(checksum&0xff));                 //发送校验位(ps.强制转换将保留低位)
            uart_write_byte(vofa_pt->BLE_UART,0x5A);                                             //发送帧尾
            return VOFA_OK;
        }

        //电脑虚拟示波器协议(v1.0)
        //备注：该协议仅接收byte类型的变量，也就是说，使用前需要先float_to_byte之类的操作(在fast_algorithm中)
        else if(vofa_protocol == EXT_PROTOCOL_DATASCOPE){
            uart_write_byte(vofa_pt->SCOPE_UART,'$');//发送帧头
            uart_write_buffer(vofa_pt->SCOPE_UART,custom_buf,28);//发送7个通道的数据（要求下位机发送的数据类型为float）
            return VOFA_OK;
        }
        return VOFA_ERR;
}

void VOFA_Sent(void){
//    VOFA_Printf("%f,%f,%f,%f,%f,%f\n",eulerAngle.yaw,gps_tau1201.direction,Direction_E,azimuth_N,azimuth_E,distance);
//        vofa_ch_data[1]=eulerAngle.yaw;
//        vofa_ch_data[2]=gps_tau1201.direction;
//        vofa_ch_data[3]=Direction_E;
//        vofa_ch_data[4]=azimuth_N;
//        vofa_ch_data[5]=azimuth_E;
//        vofa_ch_data[6]=distance;

        VOFA_Printf("%f,%f,%f,%f,%f,%f\n",eulerAngle.yaw,gps_tau1201.direction,direction_N,azimuth_N,azimuth_E,GpsOffset);
               vofa_ch_data[1]=eulerAngle.yaw;
               vofa_ch_data[2]=gps_tau1201.direction;
               vofa_ch_data[3]=direction_N;
               vofa_ch_data[4]=azimuth_N;
               vofa_ch_data[5]=azimuth_E;
               vofa_ch_data[6]=GpsOffset;
}
