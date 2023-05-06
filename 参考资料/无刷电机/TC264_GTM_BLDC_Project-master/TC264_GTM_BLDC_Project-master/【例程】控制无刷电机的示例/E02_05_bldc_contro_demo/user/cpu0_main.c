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
#include "zf_device_ips200.h"
#include "isr_config.h"
#pragma section all "cpu0_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中


// 工程导入到软件之后，应该选中工程然后点击refresh刷新一下之后再编译
// 工程默认设置为关闭优化，可以自己右击工程选择properties->C/C++ Build->Setting
// 然后在右侧的窗口中找到C/C++ Compiler->Optimization->Optimization level处设置优化等级
// 一般默认新建立的工程都会默认开2级优化，因此大家也可以设置为2级优化

// 对于TC系列默认是不支持中断嵌套的，希望支持中断嵌套需要在中断内使用 interrupt_global_enable(0); 来开启中断嵌套
// 简单点说实际上进入中断后TC系列的硬件自动调用了 interrupt_global_disable(); 来拒绝响应任何的中断，因此需要我们自己手动调用 interrupt_global_enable(0); 来开启中断的响应。

// *************************** 例程硬件连接说明 ***************************
// 如果使用的并不是逐飞科技 tc264 主板 请按照下述方式进行接线
//      模块引脚    单片机引脚
//      PWM         查看 main.c 中 PWM_CH1  宏定义 默认 P02_7 或者 P02_5
//      DIR         查看 main.c 中 DIR_CH1  宏定义 默认 P02_6 或者 P02_4
//      SPDOUT      查看 main.c 中 ENCODER1_PLUS 宏定义 默认 P10_3 或者 P33_7
//      DIROUT      查看 main.c 中 ENCODER1_DIR  宏定义 默认 P10_1 或者 P33_6
//      GND         电源地


// *************************** 例程测试说明 ***************************
// 1.核心板烧录完成本例程 主板电池供电 连接 drv8701 单驱
// 2.如果接了电机 可以看到电机周期正反转
// 3.可以在串口助手上看到如下串口信息：
//      bldc encoder counter： xxx.
// 4.如果没有接电机 使用万用表可以在主板输出排针上测量到输出电压变化
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查

// **************************** 代码区域 ****************************

#define MAX_DUTY            (25 )                                              // 最大占空比输出限制
#define DIR_CH1             (P02_6)                                            // 电机方向输出端口
#define PWM_CH1             (ATOM0_CH7_P02_7)                                  // PWM输出端口


#define ENCODER1_TIM        (TIM5_ENCODER)                                     // 编码器定时器
#define ENCODER1_PLUS       (TIM5_ENCODER_CH1_P10_3)                           // 编码器计数端口
#define ENCODER1_DIR        (TIM5_ENCODER_CH2_P10_1)                           // 编码器方向采值端口

int8 duty = 7;                                                                 // 当前占空比
bool dir = true;                                                               // 计数方向
int16 encoder;                                                        // 编码器值

int core0_main(void){
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等

    pwm_init(PWM_CH1, 1000, 0);                                                 // PWM 通道1 初始化频率1KHz 占空比初始为0
    gpio_init(DIR_CH1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                          // 初始化电机方向输出引脚
    encoder_dir_init(ENCODER1_TIM, ENCODER1_PLUS, ENCODER1_DIR);                // 初始化编码器采值引脚及定时器

    pit_ms_init(CCU60_CH0, 100);

    ips200_init(IPS200_TYPE_SPI);


    // 此处编写用户代码 例如外设初始化代码等
	cpu_wait_event_ready();         // 等待所有核心初始化完毕

	while (TRUE)
	{
        // 此处编写需要循环执行的代码


        system_delay_ms(1000);

        // 此处编写需要循环执行的代码
	}
}

IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);                                                 // 开启中断嵌套
    pit_clear_flag(CCU60_CH0);

    pwm_set_duty(PWM_CH1, duty * (PWM_DUTY_MAX / 100));                     // 计算占空比
    gpio_set_level(DIR_CH1, 0);

    encoder = encoder_get_count(ENCODER1_TIM);                               // 采集对应编码器数据
    encoder_clear_count(ENCODER1_TIM);                                          // 清除对应计数

}


#pragma section all restore
// **************************** 代码区域 ****************************

