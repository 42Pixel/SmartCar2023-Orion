/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2021,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：联系淘宝客服
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            key
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ3184284598)
 * @version         查看doc内version文件 版本说明
 * @Software        ADS v1.5.2
 * @Target core     TC264D
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-12-10
 ********************************************************************************************************************/

#include "zf_gpio.h"
#include "motor.h"
#include "hall.h"
#include "key.h"

//开关状态变量
uint8 key1_status=1;

//上一次开关状态变量
uint8 key1_last_status;

//开关标志位
uint8 key1_flag;

uint8 model_state;

void key_init(void)
{
    gpio_init(KEY1, GPI, 1, PULLUP);
}



void key_scan(void)
{
    //使用此方法优点在于，不需要使用while(1) 等待，避免处理器资源浪费
    //保存按键状态
    key1_last_status = key1_status;
    //读取当前按键状态
    key1_status = gpio_get(KEY1);

    //检测到按键按下之后  并放开置位标志位
    if(key1_status && !key1_last_status)    key1_flag = 1;

    //标志位置位之后，可以使用标志位执行自己想要做的事件
    if(key1_flag)
    {
      key1_flag = 0;//使用按键之后，应该清除标志位
      model_state=!model_state;         //更改运行模式
    }
}
