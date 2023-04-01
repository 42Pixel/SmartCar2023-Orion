/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             zf_driver_delay
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ3184284598)
* @version          查看doc内version文件 版本说明
* @Software         IAR 8.32.4 or MDK 5.28
* @Target core      MM32F3277
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-11
********************************************************************************************************************/

#ifndef _zf_driver_delay_h_
#define _zf_driver_delay_h_

#include "hal_device_registers.h"

#include "zf_common_clock.h"

void        system_delay            (uint32 time, uint32 num);

#define     system_delay_ms(time)   system_delay(1000000,time)                   // 设置延时时间 单位ms
#define     system_delay_us(time)   system_delay(time*1000,1)                    // 设置延时时间 单位us 范围0 - 16777215(0xffffff)/(DELAY_CLK*1000)ms  主频 120M systick 最大延时时间大约为 139ms
#define     system_delay_ns(time)   system_delay(time,1)                         // 设置延时时间 单位ns 范围0 - 16777215(0xffffff)*1000/(DELAY_CLK)ns  主频 120M systick 最大延时时间大约为 139ms

#endif
