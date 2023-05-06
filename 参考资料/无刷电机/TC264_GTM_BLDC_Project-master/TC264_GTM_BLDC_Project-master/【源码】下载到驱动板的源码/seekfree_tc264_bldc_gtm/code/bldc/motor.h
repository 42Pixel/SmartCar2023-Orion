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

#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "zf_common_typedef.h"
#include "zf_driver_gpio.h"
#include "zf_driver_pwm.h"
#include "gtm_pwm.h"



#define MOTOR_SPEED_OUT_PIN (ATOM1_CH2_P21_4)   // 电机旋转速度输出引脚
#define MOTOR_DIR_OUT_PIN   (P21_5          )   // 电机运行方向输出引脚

#define COMMUTATION_TIMEOUT (5000           )   // 换相超时时间  单位：10us

#define POLEPAIRS           (1              )   // 电机极对数

#define PWM_PIT_NUM         (60*FPWM        )

typedef enum
{
    REVERSE,                                    // 反转
    FORWARD,                                    // 正转
}motor_dir_enum;

typedef enum
{
    MOTOR_ENABLE,                               // 驱动使能
    MOTOR_DISABLE,                              // 驱动关闭
}motor_en_state_enum;

typedef enum
{
    MOTOR_STOP,                                 // 电机未运行
    MOTOR_RUN,                                  // 电机正在运行
}motor_run_state_enum;

typedef enum
{
    USER_INPUT,                                 // 用户输入模式   通过用户输入的PWM占空比和DIR信号控制电机
    BOARD_POTENTIOMET,                          // 板载电位器模式  使用驱动板上的电位器控制转速 方向由DIR引脚控制
}motor_run_model_enum;

typedef enum
{
    NORMAL_VOLTAGE,                             // 电池电压正常
    LOW_VOLTAGE,                                // 电池电压过低
    NO_BATTERY,                                 // 未连接电池
}battery_state_enum;

typedef struct
{
    motor_en_state_enum     en_status;          // 电机使能状态
    uint8                   brake_flag;         // 电机刹车标志位          1：触发刹车  0：未触发刹车
    motor_run_model_enum    run_model;          // 电机控制模式           USER_INPUT：用户控制  BOARD_POTENTIOMET：电位器控制
    motor_run_state_enum    run_state;          // 电机运行状态
    uint8                   locked_rotor;       // 电机是否堵转           1：电机堵转  0：电机正常旋转
    motor_dir_enum          set_dir;            // 电机当前设置方向        FORWARD：正转  REVERSE：反转
    motor_dir_enum          spin_dir;           // 电机当前旋转方向        FORWARD：正转  REVERSE：反转
    int32                   motor_speed;        // 电机当前转速
    battery_state_enum      battery_state;      // 电池当前状态

}motor_struct;


extern motor_struct motor_control;
extern uint8 phase_change;

void motor_information_out_init(void);
void motor_set_dir(void);
void speed_ctrl(speed_st speed);
void advance_switch(void);
void phase_change_manage(void);
void phase_change_time_check(void);
void motor_dir_out(void);
void motor_speed_out(void);
void motor_check(void);
void motor_init(void);

#endif /* CODE_MOTOR_H_ */
