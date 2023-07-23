#ifndef _ADC_H
#define _ADC_H

#include "common.h"

#define ADC_NUMBER              ADC_2           //定义ADC编号

#define BOARD_POTENTIOMET_PORT  ADC2_CH11_A45     //板载电位器读取通道
#define A_PHASE_PORT            ADC2_CH3_A35     //A相电流读取通道
#define B_PHASE_PORT            ADC2_CH4_A36     //B相电流读取通道
#define C_PHASE_PORT            ADC2_CH5_A37     //C相电流读取通道
#define CENTER_PHASE_PORT       ADC2_CH6_A38     //母线电流读取通道

typedef struct
{
    uint16 voltage_bus;         //母线电流
    uint16 current_a;           //A相电流
    uint16 current_b;           //B相电流
    uint16 current_c;           //C相电流
    uint16 current_board;       //板载电位器电压
}adc_struct;

extern adc_struct adc_information;



void adc_collection_init(void);
void adc_read(void);

#endif
