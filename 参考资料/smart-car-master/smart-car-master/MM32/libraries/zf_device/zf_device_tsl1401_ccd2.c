/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             zf_device_tsl1401_ccd2
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ3184284598)
* @version          查看doc内version文件 版本说明
* @Software         IAR 8.32.4 or MDK 5.28
* @Target core      MM32F3277
* @Taobao           https://seekfree.taobao.com/
* @date             2021-02-22
* @note             接线定义：
*                   ------------------------------------
*                   模块管脚            单片机管脚
*                   CLK                 查看 zf_device_tsl1401.h 中 TSL1401_CLK_PIN 宏定义
*                   SI                  查看 zf_device_tsl1401.h 中 TSL1401_SI_PIN 宏定义
*                   AO[x]               查看 zf_device_tsl1401.h 中 TSL1401_AO_PIN_BUFFER 宏定义
*                   电源引脚
*                   VCC                 3.3V电源
*                   GND                 电源地
*                   ------------------------------------
********************************************************************************************************************/

#include "zf_device_tsl1401_ccd2.h"

uint16 tsl1401_data2[TSL1401_DATA_LEN2];                                        // 对应第二个 TSL1401 数据存放数组

static uint8 tsl1401_init_state2 = 0;
uint8 tsl1401_finish_flag2;                                                     // TSL1401 数据准备就绪标志位

//-------------------------------------------------------------------------------------------------------------------
// @brief       TSL1401 线阵 TSL1401 数据采集 该函数在 isr.c 中对应 TSL1401_PIT_INDEX 的中断服务函数调用
// @param       void
// @return      void
// Sample usage:
// note:                    开源库默认使用 TIM8_PIT 那么这个函数默认在 TIM8_UP_IRQHandler 中调用
//-------------------------------------------------------------------------------------------------------------------
void tsl1401_collect_pit_handler2 (void)
{
    if(!tsl1401_init_state2)
        return;

    uint8 i = 0;

    TSL1401_CLK2(1);
    TSL1401_SI2(0);
    TSL1401_CLK2(0);
    TSL1401_SI2(1);
    TSL1401_CLK2(1);
    TSL1401_SI2(0);

    for(i=0;i<TSL1401_DATA_LEN2;i++)
    {
        TSL1401_CLK2(0);
        tsl1401_data2[i] = adc_convert(TSL1401_AO_PIN2);
        TSL1401_CLK2(1);
    }
    //采集完成标志位置1
    tsl1401_finish_flag2 = 1;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       TSL1401 线阵 TSL1401 初始化
// @param       void
// @return      void
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void tsl1401_init2 (void)     
{
    adc_init(TSL1401_AO_PIN2, TSL1401_AD_RESOLUTION2);
    gpio_init(TSL1401_CLK_PIN2, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(TSL1401_SI_PIN2, GPO, GPIO_LOW, GPO_PUSH_PULL);
    pit_init_ms(TSL1401_PIT_INDEX2, TSL1401_EXPOSURE_TIME2);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       TSL1401 线阵 CCD 图像发送至上位机查看图像
// @param       uart_n          串口号
// @param       tsl1401_index   对应接入的哪个 TSL1401
// @return      void
// Sample usage:                调用该函数前请先初始化串口
//-------------------------------------------------------------------------------------------------------------------
void tsl1401_send_data2 (uart_index_enum uart_n, uint16 *data_buffer)
{
    uint8 i;

    uart_putchar(uart_n, 0x00);
    uart_putchar(uart_n, 0xff);
    uart_putchar(uart_n, 0x01);
    uart_putchar(uart_n, 0x00);

    for(i=0; i<TSL1401_DATA_LEN2; i++)
    {
        uart_putchar(uart_n, data_buffer[i]>>8);                                // 发送高8位
        uart_putchar(uart_n, data_buffer[i]&0XFF);                              // 发送高低8位
    }
}

