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
* 文件名称          gpio
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

#include "zf_driver_pwm.h"
#include "zf_driver_delay.h"
#include "motor.h"
#include "gpio.h"

uint8 key_status = 1;   // 当前按键状态
uint8 key_last_status;  // 上一次按键状态
uint8 key_flag;         // 按键触发标志位


void led_init(void)
{
    gpio_init(LED_RUN_PIN   , GPO, 1, GPO_PUSH_PULL);       // 初始化运行指示灯
    gpio_init(LED_ERR_PIN   , GPO, 1, GPO_PUSH_PULL);       // 初始化故障指示灯
    gpio_init(LED_MODEL_PIN , GPO, 1, GPO_PUSH_PULL);       // 初始化模式指示灯
    gpio_init(EN_PIN        , GPI, 1, GPI_PULL_UP);         // 初始化使能开关

}

void led_output(void)
{
    if(!gpio_get_level(EN_FSOI_PIN) && !gpio_get_level(EN_PIN))
    {
        if(motor_control.run_state == MOTOR_RUN)
        {
            gpio_set_level(LED_RUN_PIN, 0);                 // 开启运行灯
        }
        else
        {
            gpio_set_level(LED_RUN_PIN, 1);                 // 关闭运行灯
        }
    }
    else
    {
        gpio_set_level(LED_RUN_PIN, 1);                     // 关闭运行灯
    }

    if((gpio_get_level(EN_FSOI_PIN) && !gpio_get_level(EN_PIN)) || motor_control.battery_state == LOW_VOLTAGE || motor_control.locked_rotor)
    {
        gpio_set_level(LED_ERR_PIN, 0);                     // 开启故障灯
    }
    else
    {
        gpio_set_level(LED_ERR_PIN, 1);                     // 关闭故障灯
    }

    if(motor_control.run_model)
    {
        gpio_set_level(LED_MODEL_PIN, 1);
    }
    else
    {
        gpio_set_level(LED_MODEL_PIN, 0);
    }
}

void bldc_key_init(void)
{
    gpio_init(KEY, GPI, 1, GPI_PULL_UP);
}

void bldc_key_scan(void)
{
    // 使用此方法优点在于，不需要使用while(1) 等待，避免处理器资源浪费
    // 保存按键状态
    key_last_status = key_status;
    // 读取当前按键状态
    key_status = gpio_get_level(KEY);
    // 检测松开按键的动作后   置位标志位
    if(key_status && !key_last_status)
    {
        key_flag = 1;
    }
    // 标志位 置位 之后，可以使用标志位执行自己想要做的事件
    if(key_flag == 1)
    {
        key_flag = 0;                 // 使用按键之后，应该清除标志位

        if(motor_control.run_model == USER_INPUT)
        {
            motor_control.run_model = BOARD_POTENTIOMET;
        }
        else
        {
            motor_control.run_model = USER_INPUT;
        }
    }
}
