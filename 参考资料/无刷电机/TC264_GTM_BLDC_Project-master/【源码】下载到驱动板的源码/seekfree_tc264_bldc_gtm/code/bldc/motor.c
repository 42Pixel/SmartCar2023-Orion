/*********************************************************************************************************************
* TC264 Opensourec Library 即（TC264 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是 TC264 开源库的一部分
*
* TC264 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          motor
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          ADS v1.8.0
* 适用平台          TC264D
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2023-02-01       pudding             first version
********************************************************************************************************************/

#include "IFXGTM_TIM_IN.h"
#include "ifxGtm_Tim.h"
#include "zf_common_function.h"
#include "zf_driver_delay.h"
#include "zf_driver_timer.h"
#include "bldc_config.h"
#include "move_filter.h"
#include "pwm_input.h"
#include "gpio.h"
#include "adc.h"
#include "motor.h"

motor_struct motor_control;

uint32 commutation_time_save = 0;                   // 换相时间

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     电机转速信息输出引脚初始化
// 参数说明     void
// 使用示例     motor_information_out_init();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void motor_information_out_init(void)
{
    pwm_init(MOTOR_SPEED_OUT_PIN, 50 , 0);                  // 初始化电机速度输出引脚，输出为频率变化的信号，例如电机转速为每分钟5000转，则引脚上的频率为5000Hz。
    gpio_init(MOTOR_DIR_OUT_PIN , GPO, 0, GPO_PUSH_PULL);   // 初始化电机方向输出引脚（用户端接收）
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     切换换相表
// 参数说明     void
// 使用示例     advance_switch();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void advance_switch(void)
{
    GTM_SPE0_CTRL_STAT.B.SPE_EN   = FALSE;

    if(motor_control.set_dir == FORWARD)
    {
        GTM_SPE0_OUT_PAT0.U = MOTOR_FOREWARD_0;     // 正转换相表
        GTM_SPE0_OUT_PAT1.U = MOTOR_FOREWARD_1;
        GTM_SPE0_OUT_PAT2.U = MOTOR_FOREWARD_2;
        GTM_SPE0_OUT_PAT3.U = MOTOR_FOREWARD_3;
        GTM_SPE0_OUT_PAT4.U = MOTOR_FOREWARD_4;
        GTM_SPE0_OUT_PAT5.U = MOTOR_FOREWARD_5;
    }
    else
    {
        GTM_SPE0_OUT_PAT0.U = MOTOR_REVERSAL_0;     // 反转换相表
        GTM_SPE0_OUT_PAT1.U = MOTOR_REVERSAL_1;
        GTM_SPE0_OUT_PAT2.U = MOTOR_REVERSAL_2;
        GTM_SPE0_OUT_PAT3.U = MOTOR_REVERSAL_3;
        GTM_SPE0_OUT_PAT4.U = MOTOR_REVERSAL_4;
        GTM_SPE0_OUT_PAT5.U = MOTOR_REVERSAL_5;
    }
    if(motor_control.brake_flag == 1)
    {
        GTM_SPE0_OUT_PAT0.U = MOTOR_BRAKE;          // 刹车
        GTM_SPE0_OUT_PAT1.U = MOTOR_BRAKE;
        GTM_SPE0_OUT_PAT2.U = MOTOR_BRAKE;
        GTM_SPE0_OUT_PAT3.U = MOTOR_BRAKE;
        GTM_SPE0_OUT_PAT4.U = MOTOR_BRAKE;
        GTM_SPE0_OUT_PAT5.U = MOTOR_BRAKE;
    }
    GTM_SPE0_CTRL_STAT.B.SPE_EN   = TRUE;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     设置旋转方向
// 参数说明     void
// 使用示例     motor_set_dir();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void motor_set_dir(void)
{
    if(gpio_get_level(MOTOR_DIR_IN_PIN) != motor_control.set_dir)
    {
        motor_control.set_dir = gpio_get_level(MOTOR_DIR_IN_PIN);
        advance_switch();
    }
    motor_dir_out();
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     电机占空比输出
// 参数说明     void
// 使用示例     speed_ctrl();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void speed_ctrl(speed_st speed)
{
    // 占空比不能大于周期值
    if( speed.duty <= speed.period )
    {
        GTM_TOM0_CH0_SR1.U = speed.duty     & 0xFFFF;
        GTM_TOM0_CH0_SR0.U = speed.period   & 0xFFFF;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     计算当前的速度
// 参数说明     void
// 使用示例     calc_speed();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
inline void calc_speed(void)
{
    //转速计算
    int32 speed;

    // 电机转速计算说明
    // 2.commutation_time_save是统计电机单次换相使用了多少时间
    // 3.通常电机转速我们都用RPM表示，RPM表示每分钟电机的转速
    // 3.电机转一圈需要换相的次数等于 电机极对数*6
    // 4.因此电机转速等于 模块时钟/单次换相时间/(电机极对数 * 6) * 60，这样可以得到电机每分钟的转速

    speed = FCY / POLEPAIRS / commutation_time_save * 10;

    // 电机反转的时候需要对速度取反
    if(FORWARD == motor_control.spin_dir)
    {
        move_filter_calc(&speed_filter, speed);
    }
    else
    {
        move_filter_calc(&speed_filter, -speed);
    }


}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     换相处理 保存在上一相位等待的时间
// 参数说明     void
// 使用示例     phase_change_manage();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void phase_change_manage(void)
{
    commutation_time_save = system_getval();
    system_start();

    calc_speed();
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     换相超时处理
// 参数说明     void
// 使用示例     phase_change_time_check();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void phase_change_time_check(void)
{
    if(system_getval() > (COMMUTATION_TIMEOUT * 1000))
    {
        commutation_time_save = COMMUTATION_TIMEOUT;

        move_filter_init(&speed_filter);                            // 滑动平均滤波初始化
        motor_control.run_state = MOTOR_STOP;
    }
    else
    {
        motor_control.run_state = MOTOR_RUN;
    }
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     电机转动方向输出
// 参数说明     void
// 使用示例     motor_dir_out();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void motor_dir_out(void)
{
    switch(spe_g.hall_now)
    {
        case 1:
        {
            if      (spe_g.hall_last == 3) motor_control.spin_dir = FORWARD;
            else if (spe_g.hall_last == 5) motor_control.spin_dir = REVERSE;
            break;
        }
        case 2:
        {
            if      (spe_g.hall_last == 6) motor_control.spin_dir = FORWARD;
            else if (spe_g.hall_last == 3) motor_control.spin_dir = REVERSE;
            break;
        }
        case 3:
        {
            if      (spe_g.hall_last == 2) motor_control.spin_dir = FORWARD;
            else if (spe_g.hall_last == 1) motor_control.spin_dir = REVERSE;
            break;
        }
        case 4:
        {
            if      (spe_g.hall_last == 5) motor_control.spin_dir = FORWARD;
            else if (spe_g.hall_last == 6) motor_control.spin_dir = REVERSE;
            break;
        }
        case 5:
        {
            if      (spe_g.hall_last == 1) motor_control.spin_dir = FORWARD;
            else if (spe_g.hall_last == 4) motor_control.spin_dir = REVERSE;
            break;
        }
        case 6:
        {
            if      (spe_g.hall_last == 4) motor_control.spin_dir = FORWARD;
            else if (spe_g.hall_last == 2) motor_control.spin_dir = REVERSE;
            break;
        }
    }

    gpio_set_level(MOTOR_DIR_OUT_PIN, motor_control.spin_dir);
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     电机速度输出
// 参数说明     void
// 使用示例     motor_speed_out();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void motor_speed_out(void)
{
    motor_control.motor_speed = speed_filter.data_average;

    if(motor_control.motor_speed)
    {
        pwm_set_freq(MOTOR_SPEED_OUT_PIN, func_abs(motor_control.motor_speed), 5000);           // 频率更改函数并未在开源库中写入 需要使用的同学自行将此函数更新到开源库
    }
    else
    {
        pwm_set_freq(MOTOR_SPEED_OUT_PIN, 1000, 0);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     电机状态检测
// 参数说明     void
// 使用示例     motor_check();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void motor_check(void)
{
    static float motor_protect_current = 0;
    static uint8 last_brake_flag = 0;

    motor_protect_current = (motor_protect_current * 19 +  adc_information.voltage_bus) / 20;   // 对母线电流低通滤波

    if(motor_protect_current > MOTOR_LOCKED_VALUE || motor_control.locked_rotor == 1)           // 母线电流超过堵转阈值
    {
        motor_control.locked_rotor = 1;
        spe_g.speed.duty = 0;
    }

    last_brake_flag = motor_control.brake_flag;                         // 保存上一次的刹车状态

    if(spe_g.speed.duty == 0 || motor_control.locked_rotor == 1)        // 当占空比为0或者堵转时将刹车标志位置位并清除占空比
    {
        spe_g.speed.duty = 0;
        motor_control.brake_flag = 1;
    }
    else
    {
        motor_control.brake_flag = 0;
#if BATTERY_PROTECT == 1
        if(motor_control.battery_state == LOW_VOLTAGE)                  // 当电压过低时将刹车标志位置位并清除占空比
        {
            spe_g.speed.duty = 0;
            motor_control.brake_flag = 1;
        }
#endif
    }

    if(last_brake_flag != motor_control.brake_flag)                     // 如果刹车标志位有变化，则更换换相数组
    {
#if BLDC_BRAKE_ENABLE == 1                                              // 启动刹车
        advance_switch();
#endif
    }

}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     电机参数初始化
// 参数说明     void
// 使用示例     motor_init();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void motor_init(void)
{
    motor_control.en_status  = gpio_get_level(EN_PIN);              // 读取当前使能状态
    motor_control.run_model  = USER_INPUT;                          // 初始默认为用户输入模式
    motor_control.set_dir    = gpio_get_level(MOTOR_DIR_IN_PIN);    // 读取方向信息
    motor_control.brake_flag = 0;
    advance_switch();

}
