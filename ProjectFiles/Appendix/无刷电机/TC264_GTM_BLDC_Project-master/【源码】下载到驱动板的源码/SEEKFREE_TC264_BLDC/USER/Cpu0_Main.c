/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            main
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ3184284598)
 * @version         查看doc内version文件 版本说明
 * @Software        ADS v1.2.2
 * @Target core     TC264D
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-3-23
 ********************************************************************************************************************/


//BLDC

#include "gpio.h"
#include "adc.h"
#include "hall.h"
#include "move_filter.h"
#include "key.h"
#include "motor.h"
#include "pid.h"
#include "headfile.h"
#include "ccu6_pwm.h"
#include "pwm_input.h"
#pragma section all "cpu0_dsram"
//将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中


//特别说明：该开源项目仅用于各参赛同学在设计自己的小车时进行参考，硬件和软件都不能够直接拷贝使用在自己的作品中，请大家参考后重新完成硬件和软件的设计制作。
//特别说明：该开源项目仅用于各参赛同学在设计自己的小车时进行参考，硬件和软件都不能够直接拷贝使用在自己的作品中，请大家参考后重新完成硬件和软件的设计制作。
//特别说明：该开源项目仅用于各参赛同学在设计自己的小车时进行参考，硬件和软件都不能够直接拷贝使用在自己的作品中，请大家参考后重新完成硬件和软件的设计制作。

int core0_main(void)
{
    //获取时钟频率  务必保留
    get_clk();

    //初始化LED引脚
    led_init();

    //初始化按键引脚
    key_init();

    //初始化adc通道，adc用于采集电源电压、母线电流、相电流的
    adc_collection_init();

    //霍尔初始化
    hall_init();

    //滑动平均滤波初始化
    move_filter_init(&speed_filter);

    //初始化输出速度与方向信息的引脚
    motor_information_out_init();

    //输入信号捕获初始化
    pwm_input_init();

    //电机初始化
    motor_init();

    //PID参数初始化
    closed_loop_pi_init();

    //初始化定时器,用于输出互补PWM
    ccu6_pwm_init();

    //初始化定时器,用于计算占空比
    pit_interrupt_ms(CCU6_0, PIT_CH0, 5);

    enableInterrupts();
    while(1){

        led_output();       //根据当前状态点亮或者熄灭LED灯

        //发送数据到虚拟示波器 虚拟示波器下载链接 https://pan.baidu.com/s/198CMXTZsbI3HAEqNXDngBw
        data_conversion((uint16)speed_filter.data_average, pwm_in_duty, hall_value_now, duty, virtual_scope_data);
        uart_putbuff(UART_0, virtual_scope_data, sizeof(virtual_scope_data));  //数据转换完成后，使用串口发送将数组的内容发送出去
    }
}

#pragma section all restore


