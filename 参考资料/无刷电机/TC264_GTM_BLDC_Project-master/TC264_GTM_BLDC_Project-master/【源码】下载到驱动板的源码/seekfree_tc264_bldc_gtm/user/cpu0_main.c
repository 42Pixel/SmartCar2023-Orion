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
* 文件名称          cpu0_main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          ADS v1.8.0
* 适用平台          TC264D
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2022-09-15       pudding            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"
#pragma section all "cpu0_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中


// 工程导入到软件之后，应该选中工程然后点击refresh刷新一下之后再编译
// 工程默认设置为关闭优化，可以自己右击工程选择properties->C/C++ Build->Setting
// 然后在右侧的窗口中找到C/C++ Compiler->Optimization->Optimization level处设置优化等级
// 一般默认新建立的工程都会默认开2级优化，因此大家也可以设置为2级优化

// 对于TC系列默认是不支持中断嵌套的，希望支持中断嵌套需要在中断内使用 interrupt_global_enable(0); 来开启中断嵌套
// 简单点说实际上进入中断后TC系列的硬件自动调用了 interrupt_global_disable(); 来拒绝响应任何的中断，因此需要我们自己手动调用 interrupt_global_enable(0); 来开启中断的响应。

// 本程序为英飞凌GTM无数驱动程序，可以基于本程序学习研究无刷电机的六步换相驱动原理，使用前请务必阅读无刷驱动说明书
// 本程序为英飞凌GTM无数驱动程序，可以基于本程序学习研究无刷电机的六步换相驱动原理，使用前请务必阅读无刷驱动说明书
// 本程序为英飞凌GTM无数驱动程序，可以基于本程序学习研究无刷电机的六步换相驱动原理，使用前请务必阅读无刷驱动说明书

// **************************** 代码区域 ****************************
int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等

    led_init();                     // 初始化LED引脚

    bldc_key_init();                // 初始化按键引脚

    adc_collection_init();          // AD采值引脚初始化

    move_filter_init(&speed_filter);// 初始化速度平滑滤波

    motor_information_out_init();   // 初始化运行信息输出端口

    pwm_input_init();               // 初始化输入捕获

    gtm_bldc_init();                // 初始化GTM模块

    motor_init();                   // 电机初始参数配置

    pit_ms_init(CCU61_CH1, 5);      // 周期中断初始化

    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready();         // 等待所有F核心初始化完毕
    while (TRUE)
    {
        // 此处编写需要循环执行的代码

        led_output();       // 根据当前状态点亮或者熄灭LED灯

        virtual_oscilloscope_data_conversion(
                (int16)motor_control.motor_speed/10,  // 转速信息
                (int16)spe_g.speed.duty,                        // 输入的占空比信息
                (int16)(adc_information.voltage_bus * 1000),    // 母线电流信息
                (int16)(adc_information.battery_voltage * 100));// 电池电压
        uart_write_buffer(UART_0, virtual_oscilloscope_data, sizeof(virtual_oscilloscope_data));  // 数据转换完成后，使用串口发送将数组的内容发送出去

        // 此处编写需要循环执行的代码
    }
}

#pragma section all restore
// **************************** 代码区域 ****************************

