/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2021,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：联系淘宝客服
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            gpio
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ3184284598)
 * @version         查看doc内version文件 版本说明
 * @Software        ADS v1.5.2
 * @Target core     TC264D
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-12-10
 ********************************************************************************************************************/

#include "zf_gpio.h"
#include "ccu6_pwm.h"
#include "hall.h"
#include "key.h"
#include "zf_gtm_pwm.h"
#include "gpio.h"




void led_init(void)
{
    gpio_init(LED_RUN_PIN, GPO, 1, PUSHPULL);           //初始化运行指示灯
    gpio_init(LED_ERR_PIN, GPO, 1, PUSHPULL);           //初始化报错指示灯
    gpio_init(LED_MODEL_PIN, GPO, 1, PUSHPULL);         //初始化按键
    gpio_init(EN_PIN, GPI, 1, PULLUP);                  //初始化运行指示灯
}


void led_output(void)
{
    if(ccu61_get_trap_flag() && gpio_get(EN_PIN))
    {
        gpio_set(LED_ERR_PIN,0);    //开启故障灯
        gpio_set(LED_RUN_PIN,1);    //关闭运行灯
    }
    else
    {
        gpio_set(LED_ERR_PIN,1);    //关闭故障灯
        if(motor_state==MOTOR_RUN) gpio_set(LED_RUN_PIN,0); //开启运行灯
        else gpio_set(LED_RUN_PIN,1);       //关闭运行灯
    }
    if(model_state)
    {
        gpio_set(LED_MODEL_PIN,1);
    }
    else
    {
        gpio_set(LED_MODEL_PIN,0);
    }
}
