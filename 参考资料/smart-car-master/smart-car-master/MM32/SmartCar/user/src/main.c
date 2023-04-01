#include "zf_common_headfile.h"

int main(void){
    clock_init(SYSTEM_CLOCK_120M);                                                // 初始化芯片时钟 工作频率为 120MHz
    debug_init();                                                                      // 初始化默认 Debug UART
    GUI_Init();
    icm20602_init();
    icmZeroInit();
    gps_init();
    Motor_Init();                                                                      //电机初始化
    Key_Init();                                                                        //按键初始化
    Servo_Init();
    HWT101_Init();

    system_delay_ms(500);

    pit_init_ms(TIM1_PIT,5);                                                    // 编码器中断 5ms
    pit_init_ms(TIM6_PIT, 10);
    pit_init_ms(TIM8_PIT, 1);

    interrupt_set_priority(TIM8_UP_IRQn, 1);
    interrupt_set_priority(TIM1_UP_IRQn, 0);

    uart_init(UART_6,115200,UART6_TX_C6,UART6_RX_C7); 		// 初始化串口6 波特率115200
    // VOFA+
    VOFA* VOFA_pt = vofa_create();       											//创建VOFA对象
    vofa_init(VOFA_pt,                  										//初始化当前的vofa对象
              vofa_ch_data,ch_sz,
              vofa_image,image_sz,
              custom_buf,custom_sz,
              cmd_rxbuf,cmd_sz,
              UART_6,UART_6,UART_6);


    system_delay_ms(100);

    while(1){
        GUI_Show();

        if(gps_tau1201_flag){
            gps_tau1201_flag = 0;
            gps_data_parse();      										         //开始解析数据
        }

        INS();
        VOFA_Send();

//        sendcmd(ACCCMD);
//        uart_write_string(UART_8,"i lvoe you");
//        uart_query_byte(UART_3,&text[])
    }

}