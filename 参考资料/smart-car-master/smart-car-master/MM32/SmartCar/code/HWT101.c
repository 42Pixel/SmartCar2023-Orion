//
// Created by JayLe on 2022/7/19.
//
#include "HWT101.h"

#define UART_PRIORITY

struct SAcc 		stcAcc;
struct SGyro 		stcGyro;
struct SAngle 	    stcAngle;
float yaw;

uint8 YAWCMD[3] = {0XFF,0XAA,0X52};
uint8 ACCCMD[3] = {0XFF,0XAA,0X67};
char SLEEPCMD[3] = {0XFF,0XAA,0X60};
char UARTMODECMD[3] = {0XFF,0XAA,0X61};
char IICMODECMD[3] = {0XFF,0XAA,0X62};

void HWT101_Init(void){
    uart_init(UART_8,9600, UART8_TX_E1 ,UART8_RX_E0);
    uart_rx_interrupt(UART_8,1);
    interrupt_set_priority(UART8_IRQn, 0);


    sendcmd(ACCCMD);//等待模块内部自动校准好，模块内部会自动计算需要一定的时间
    system_delay_ms(500);
    sendcmd(YAWCMD);
}

//CopeSerialData为串口8中断调用函数，串口每收到一个数据，调用一次这个函数。
void CopeSerial8Data() {
    static uint8 get_data = 0;
    static unsigned char ucRxBuffer[64];
    static unsigned char ucRxCnt = 0;
    uart_query_byte(UART_8, &get_data);                                     // 接收数据 查询式 有数据会返回 TRUE 没有数据会返回 FALSE
    ucRxBuffer[ucRxCnt++] = get_data;
    if (ucRxBuffer[0]!=0x55){        //数据头不对，则重新开始寻找0x55数据头
        ucRxCnt=0;
        return;
    }
    if (ucRxCnt<11) {return;}//数据不满11个，则返回
    else
    {
        switch(ucRxBuffer[1])//判断数据是哪种数据，然后将其拷贝到对应的结构体中，有些数据包需要通过上位机打开对应的输出后，才能接收到这个数据包的数据
        {
            //memcpy为编译器自带的内存拷贝函数，需引用"string.h"，将接收缓冲区的字符拷贝到数据结构体里面，从而实现数据的解析。
            case 0x52:	memcpy(&stcGyro,&ucRxBuffer[2],8);break;
            case 0x53:	memcpy(&stcAngle,&ucRxBuffer[2],8);break;
        }
        ucRxCnt=0;//清空缓存区
    }

    yaw=(float)stcAngle.Angle[2]/32768*180;
}


void sendcmd(uint8 cmd[]){               //给HWT101发送指令
    char i;
    for(i=0;i<3;i++)
        uart_write_buffer(UART_8,&cmd[i],1);
}