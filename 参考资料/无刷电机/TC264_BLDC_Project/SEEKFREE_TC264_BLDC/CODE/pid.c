/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2021,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：联系淘宝客服
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            pid
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ3184284598)
 * @version         查看doc内version文件 版本说明
 * @Software        ADS v1.5.2
 * @Target core     TC264D
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-12-10
 ********************************************************************************************************************/

#include "ccu6_pwm.h"
#include "SEEKFREE_FUN.h"
#include "motor.h"
#include "pid.h"

closed_loop_struct closed_loop;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PI闭环计算
//  @param      read_speed  当前速度
//  @return     void
//  @since
//-------------------------------------------------------------------------------------------------------------------
int32 closed_loop_pi_calc(int32 read_error)
{

    closed_loop.error = read_error;

    closed_loop.pout = closed_loop.error * (closed_loop.kp + (float)myabs(closed_loop.error/1000)/1800);

    //积分系数根据误差进行动态调节
    closed_loop.iout += closed_loop.error * (closed_loop.ki + (float)myabs(closed_loop.error/1000)/38000);

    //积分限幅
    closed_loop.iout = (float)limit_ab(closed_loop.out,0,closed_loop.out_max);

    //如果目标速度为0或者电机被关闭则清除积分
    if((0 == motor_control.set_speed )|| (ccu61_get_trap_flag()))
    {
        closed_loop.iout = 0;
    }

    closed_loop.out = closed_loop.iout + closed_loop.pout;

    //输出限幅
    closed_loop.out = limit_ab(closed_loop.out,0,closed_loop.out_max);

    return closed_loop.out;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      PI闭环计算初始化
//  @param      void
//  @return     void
//  @since
//-------------------------------------------------------------------------------------------------------------------
void closed_loop_pi_init(void)
{
    closed_loop.out_max = PWM_PRIOD_LOAD;
    closed_loop.kp = 0.001;
    closed_loop.ki = 0.00001;
    closed_loop.out = 0;
    closed_loop.real_speed = 0;
}
