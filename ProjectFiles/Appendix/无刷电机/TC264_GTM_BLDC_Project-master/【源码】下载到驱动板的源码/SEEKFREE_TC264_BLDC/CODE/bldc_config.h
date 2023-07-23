/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2021,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：联系淘宝客服
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            bldc_config
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ3184284598)
 * @version         查看doc内version文件 版本说明
 * @Software        ADS v1.5.2
 * @Target core     TC264D
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-12-10
 ********************************************************************************************************************/

#ifndef _BLDC_CONFIG_H
#define _BLDC_CONFIG_H


//0：L车K车3650-2860无刷有感电机
//1：K1车专用无刷电机
#define MOTOR_TYPE       1

//0：不开启刹车功能     1：开启
#define BLDC_BRAKE_ENABLE       1

//0：不开启速度闭环     1：开启
#define BLDC_CLOSE_LOOP_ENABLE  0           //鉴于电机所带负载不同 默认不开启PID计算



#if BLDC_CLOSE_LOOP_ENABLE==1
    //设置最大转速
    #define BLDC_MAX_SPEED          32000

#endif





#endif
