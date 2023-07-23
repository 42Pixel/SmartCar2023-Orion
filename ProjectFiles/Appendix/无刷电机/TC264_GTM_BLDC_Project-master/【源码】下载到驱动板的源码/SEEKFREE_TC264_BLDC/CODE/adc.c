/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2021,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：联系淘宝客服
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            adc
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ3184284598)
 * @version         查看doc内version文件 版本说明
 * @Software        ADS v1.5.2
 * @Target core     TC264D
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-12-10
 ********************************************************************************************************************/

#include "zf_vadc.h"
#include "adc.h"


adc_struct adc_information;

void adc_collection_init(void)
{
    adc_init(ADC_NUMBER, BOARD_POTENTIOMET_PORT);   //板载电位器
    adc_init(ADC_NUMBER, A_PHASE_PORT);             //A相电流
    adc_init(ADC_NUMBER, B_PHASE_PORT);             //B相电流
    adc_init(ADC_NUMBER, C_PHASE_PORT);             //C相电流
    adc_init(ADC_NUMBER, CENTER_PHASE_PORT);        //母线电流
}

void adc_read(void)
{
    adc_information.current_board   = adc_convert(ADC_NUMBER, BOARD_POTENTIOMET_PORT, ADC_12BIT);   //获取板载电位器电压
    adc_information.current_a       = adc_convert(ADC_NUMBER, A_PHASE_PORT, ADC_12BIT);             //获取A相电流
    adc_information.current_b       = adc_convert(ADC_NUMBER, B_PHASE_PORT, ADC_12BIT);             //获取B相电流
    adc_information.current_c       = adc_convert(ADC_NUMBER, C_PHASE_PORT, ADC_12BIT);             //获取C相电流
    adc_information.voltage_bus     = adc_convert(ADC_NUMBER, CENTER_PHASE_PORT, ADC_12BIT);        //获取母线电流
}
