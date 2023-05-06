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

// *************************** 例程硬件连接说明 ***************************
// 使用 tc264 核心板直接接线进行测试
//      模块管脚            单片机管脚
//      1-A                 P02_4或者P21_2
//      1-B                 P02_5或者P21_3
//      GND                 GND
//      2-A                 P02_6或者P21_4
//      2-B                 P02_7或者P21_5
//      GND                 GND
//      接线端子 +          电池正极
//      接线端子 -          电池负极
// 使用 tc264 学习主板进行测试
//      将模块的电源接线端子与主板的驱动供电端子连接
//      将模块的信号接口使用配套灰排线与主板电机信号接口连接 请注意接线方向 不确定方向就是用万用表确认一下 引脚参考上方核心板连接
//      将主板与供电电池正确连接


// *************************** 例程测试说明 ***************************
// 1.核心板烧录完成本例程 主板电池供电 连接hip4082双驱
// 2.如果接了电机 可以看到电机周期正反转
// 3.如果没有接电机 使用万用表可以在驱动电机输出端子上测量到输出电压变化
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查


// **************************** 代码区域 ****************************
#define MAX_DUTY            (50)                                                // 最大 MAX_DUTY% 占空比
#define PWM_R1              (ATOM0_CH4_P02_4)
#define PWM_R2              (ATOM0_CH5_P02_5)
#define PWM_R3              (ATOM0_CH0_P21_2)
#define PWM_R4              (ATOM0_CH1_P21_3)

#define PWM_L1              (ATOM0_CH6_P02_6)
#define PWM_L2              (ATOM0_CH7_P02_7)
#define PWM_L3              (ATOM0_CH2_P21_4)
#define PWM_L4              (ATOM0_CH3_P21_5)

int8 duty = 0;
bool dir = true;

// **************************** 代码区域 ****************************


int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等

    pwm_init(PWM_R1, 17000, 0);                                                 // PWM 通道 R1 初始化频率 17KHz 占空比初始为 0
    pwm_init(PWM_R2, 17000, 0);                                                 // PWM 通道 R2 初始化频率 17KHz 占空比初始为 0
    pwm_init(PWM_R3, 17000, 0);                                                 // PWM 通道 R1 初始化频率 17KHz 占空比初始为 0
    pwm_init(PWM_R4, 17000, 0);                                                 // PWM 通道 R2 初始化频率 17KHz 占空比初始为 0
    pwm_init(PWM_L1, 17000, 0);                                                 // PWM 通道 L1 初始化频率 17KHz 占空比初始为 0
    pwm_init(PWM_L2, 17000, 0);                                                 // PWM 通道 L2 初始化频率 17KHz 占空比初始为 0
    pwm_init(PWM_L3, 17000, 0);                                                 // PWM 通道 L1 初始化频率 17KHz 占空比初始为 0
    pwm_init(PWM_L4, 17000, 0);                                                 // PWM 通道 L2 初始化频率 17KHz 占空比初始为 0
    // 此处编写用户代码 例如外设初始化代码等
	cpu_wait_event_ready();         // 等待所有核心初始化完毕
	while (TRUE)
	{
        // 此处编写需要循环执行的代码

        if(duty >= 0)                                                           // 左侧正转
        {
            pwm_set_duty(PWM_L1, duty * (PWM_DUTY_MAX / 100));                  // 计算占空比
            pwm_set_duty(PWM_L3, duty * (PWM_DUTY_MAX / 100));                  // 计算占空比
            pwm_set_duty(PWM_L2, 0);                                            // 同一时间 一个电机只能输出一个 PWM 另一通道保持低电平
            pwm_set_duty(PWM_L4, 0);                                            // 同一时间 一个电机只能输出一个 PWM 另一通道保持低电平

            pwm_set_duty(PWM_R1, duty * (PWM_DUTY_MAX / 100));                  // 计算占空比
            pwm_set_duty(PWM_R3, duty * (PWM_DUTY_MAX / 100));                  // 计算占空比
            pwm_set_duty(PWM_R2, 0);                                            // 同一时间 一个电机只能输出一个 PWM 另一通道保持低电平
            pwm_set_duty(PWM_R4, 0);                                            // 同一时间 一个电机只能输出一个 PWM 另一通道保持低电平
        }
        else                                                                    // 左侧反转
        {
            pwm_set_duty(PWM_L1, 0);                                            // 同一时间 一个电机只能输出一个 PWM 另一通道保持低电平
            pwm_set_duty(PWM_L3, 0);                                            // 同一时间 一个电机只能输出一个 PWM 另一通道保持低电平
            pwm_set_duty(PWM_L2, (-duty) * (PWM_DUTY_MAX / 100));               // 计算占空比
            pwm_set_duty(PWM_L4, (-duty) * (PWM_DUTY_MAX / 100));               // 计算占空比

            pwm_set_duty(PWM_R1, 0);                                            // 同一时间 一个电机只能输出一个 PWM 另一通道保持低电平
            pwm_set_duty(PWM_R3, 0);                                            // 同一时间 一个电机只能输出一个 PWM 另一通道保持低电平
            pwm_set_duty(PWM_R2, (-duty) * (PWM_DUTY_MAX / 100));               // 计算占空比
            pwm_set_duty(PWM_R4, (-duty) * (PWM_DUTY_MAX / 100));               // 计算占空比
        }
        if(dir)                                                                 // 根据方向判断计数方向 本例程仅作参考
        {
            duty ++;                                                            // 正向计数
            if(duty >= MAX_DUTY)                                                // 达到最大值
                dir = false;                                                    // 变更计数方向
        }
        else
        {
            duty --;                                                            // 反向计数
            if(duty <= -MAX_DUTY)                                               // 达到最小值
                dir = true;                                                     // 变更计数方向
        }
        system_delay_ms(50);

        // 此处编写需要循环执行的代码
	}
}

#pragma section all restore
// **************************** 代码区域 ****************************
// *************************** 例程常见问题说明 ***************************
// 遇到问题时请按照以下问题检查列表检查
// 问题1：电机不转或者模块输出电压无变化
//      如果使用主板测试，主板必须要用电池供电
//      检查模块是否正确连接供电 必须使用电源线供电 不能使用杜邦线
//      查看程序是否正常烧录，是否下载报错，确认正常按下复位按键
//      万用表测量对应 PWM 引脚电压是否变化，如果不变化证明程序未运行，或者引脚损坏，或者接触不良 联系技术客服
