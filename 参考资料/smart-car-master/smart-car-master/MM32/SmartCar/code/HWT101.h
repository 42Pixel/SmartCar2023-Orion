//
// Created by JayLe on 2022/7/19.
//


#ifndef _HWT101_H
#define _HWT101_H

#include "zf_common_headfile.h"
#include "string.h"

//#define UART_INDEX              UART_8
////#define UART_BAUDRATE           DEBUG_UART_BAUDRATE                          // 默认 115200
//#define UART_TX_PIN             UART8_TX_E1                        // 默认 UART1_TX_A9
//#define UART_RX_PIN             UART8_RX_E0                           // 默认 UART1_RX_A10
////#define UART_PRIORITY           (UART3_IRQn)

extern uint8 uart_get_data[64];                                                        // 串口接收数据缓冲区
extern uint8 fifo_get_data[64];
extern uint32 fifo_data_count ;

//
extern uint8 get_data;                                                             // 接收数据变量
                                                 // fifo 数据个数
extern fifo_struct uart_data_fifo;



struct SAcc{
    short a[3];
    short T;
};

struct SGyro{
    short w[3];
    short T;
};

struct SAngle{
    short Angle[3];
    short T;
};


//extern struct SAcc 		    stcAcc;
extern struct SGyro 		stcGyro;
extern struct SAngle 	    stcAngle;

extern float yaw;

//extern void CopeSerial8Data(unsigned char ucData);
extern void CopeSerial8Data();
extern void HWT101_Init(void);

void sendcmd(uint8 cmd[]);

#endif //MM32_HWT101_H
