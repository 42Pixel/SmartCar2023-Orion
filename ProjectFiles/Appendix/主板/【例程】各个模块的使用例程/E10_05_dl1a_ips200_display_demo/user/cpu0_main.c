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
// 接入2寸IPS模块
//      模块管脚            单片机管脚
//      BL                  查看 zf_device_ips200_parallel8.h 中 IPS200_BL_PIN 宏定义  默认 P15_3
//      CS                  查看 zf_device_ips200_parallel8.h 中 IPS200_CS_PIN 宏定义  默认 P15_5
//      RST                 查看 zf_device_ips200_parallel8.h 中 IPS200_RST_PIN 宏定义 默认 P15_1
//      RS                  查看 zf_device_ips200_parallel8.h 中 IPS200_RS_PIN 宏定义  默认 P15_0
//      WR                  查看 zf_device_ips200_parallel8.h 中 IPS200_WR_PIN 宏定义  默认 P15_2
//      RD                  查看 zf_device_ips200_parallel8.h 中 IPS200_RD_PIN 宏定义  默认 P15_4
//      D0-D7               查看 zf_device_ips200_parallel8.h 中 IPS200_Dx_PIN 宏定义  默认 P11_9/P11_10/P11_11/P11_12/P13_0/P13_1/P13_2/P13_3
//      GND                 核心板电源地 GND
//      3V3                 核心板 3V3 电源
// 接入DL1A激光测距模块 对应主板 TOF 接口 请注意线序
//      模块管脚            单片机管脚
//      SCL                 查看 zf_device_dl1a.h 中 DL1A_SCL_PIN  宏定义
//      SDA                 查看 zf_device_dl1a.h 中 DL1A_SDA_PIN  宏定义
//      VCC                 5V 电源
//      GND                 电源地

// *************************** 例程测试说明 ***************************
// 1.核心板烧录完成本例程 将核心板插在主板上 插到底 DL1A接在主板的 TOF 模块接口 注意线序 屏幕插入主板屏幕接口
// 2.主板上电 或者核心板链接完毕后上电 核心板按下复位按键
// 3.屏幕会显示DL1A模块的距离信息
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查

// **************************** 代码区域 ****************************
#define LED1                    (P20_9)
#define IPS200_TYPE             (IPS200_TYPE_PARALLEL8)                         // 双排排针 并口两寸屏 这里宏定义填写 IPS200_TYPE_PARALLEL8
                                                                                // 单排排针 SPI 两寸屏 这里宏定义填写 IPS200_TYPE_SPI
int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等

    gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // 初始化 LED1 输出 默认高电平 推挽输出模式
    ips200_init(IPS200_TYPE);
    while(1)
    {
        if(dl1a_init())
            gpio_toggle_level(LED1);                                            // 翻转 LED 引脚输出电平 控制 LED 亮灭 初始化出错这个灯会闪的很慢
        else
            break;
        system_delay_ms(1000);                                                  // 闪灯表示异常
    }
    ips200_clear();
    ips200_show_string(0, 0, "dl1a distance:");
    ips200_show_string(160, 0, "mm");
    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready();         // 等待所有核心初始化完毕
    while (TRUE)
    {
        // 此处编写需要循环执行的代码

        dl1a_get_distance();
        if(dl1a_finsh_flag == 1)
        {
            dl1a_finsh_flag = 0;
            ips200_show_uint(120, 0, dl1a_distance_mm, 4);
        }
        system_delay_ms(20);


        // 此处编写需要循环执行的代码
    }
}

#pragma section all restore
// **************************** 代码区域 ****************************

// *************************** 例程常见问题说明 ***************************
// 遇到问题时请按照以下问题检查列表检查
// 问题1：屏幕不显示
//      如果使用主板测试，主板必须要用电池供电 检查屏幕供电引脚电压
//      检查屏幕是不是插错位置了 检查引脚对应关系
//      如果对应引脚都正确 检查一下是否有引脚波形不对 需要有示波器
//      无法完成波形测试则复制一个GPIO例程将屏幕所有IO初始化为GPIO翻转电平 看看是否受控
// 问题2：屏幕显示内容 但没数据
//      如果接线都正常 查看LED灯是否闪烁 如果闪烁证明初始化失败 检查模块接线是否正常 线序是否正确
//      如果模块接线都正常且LED灯不闪烁 联系技术客服即可
// 问题3：有数据但是数据不正确
//      模块的测量范围为0 - 1200 mm 超过此距离得到的值为8191 属于无效数据
//      检查接线是否存在接触不良或者走线过长
