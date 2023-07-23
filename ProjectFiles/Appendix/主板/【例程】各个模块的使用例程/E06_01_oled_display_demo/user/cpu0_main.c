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
//      模块管脚            单片机管脚
//      D0                  zf_device_oled.h 中 OLED_D0_PIN  宏定义 默认 P15_3
//      D1                  zf_device_oled.h 中 OLED_D1_PIN  宏定义 默认 P15_5
//      RES                 zf_device_oled.h 中 OLED_RES_PIN 宏定义 默认 P15_1
//      DC                  zf_device_oled.h 中 OLED_DC_PIN  宏定义 默认 P15_0
//      CS                  zf_device_oled.h 中 OLED_CS_PIN  宏定义 默认 P15_2
//      GND                 核心板电源地 GND
//      3V3                 核心板 3V3 电源



// *************************** 例程测试说明 ***************************
// 1.核心板烧录本例程 插在主板上 OLED显示模块插在主板的屏幕接口排座上 请注意引脚对应 不要插错
// 2.电池供电 上电后OLED屏幕回亮起 显示字符数字浮点数和波形图
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查

// **************************** 代码区域 ****************************


int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等

    uint16_t data[128];
    uint16_t data_index = 0;
    for( ; data_index < 64; data_index ++)
       data[data_index] = data_index;
    for(data_index = 64; data_index < 128; data_index ++)
       data[data_index] = 128 - data_index;

    oled_init();

    // 此处编写用户代码 例如外设初始化代码等
	cpu_wait_event_ready();         // 等待所有核心初始化完毕
	while (TRUE)
	{
        // 此处编写需要循环执行的代码

	    oled_clear();
        oled_show_string(0, 0, "SEEKFREE");                                     // 显示字符串
        oled_show_float(0, 1, -13.141592, 1, 6);                                // 显示 float 数据 1bit 整数 6bit 小数 应当显示 -3.141592 总共会有 9 个字符的显示占位
        oled_show_float(64, 1, 13.141592, 8, 4);                                // 显示 float 数据 8bit 整数 4bit 小数 应当显示 13.1415 总共会有 14 个字符的显示占位 后面会有 5 个字符的空白占位
        oled_show_int(0, 2, -127, 2);                                           // 显示 int8 数据
        oled_show_uint(64, 2, 255, 4);                                          // 显示 uint8 数据
        oled_show_int(64, 3, -32768, 4);                                        // 显示 int16 数据
        oled_show_uint(0, 3, 65535, 6);                                         // 显示 uint16 数据
        oled_show_int(0, 4, -2147483648, 8);                                    // 显示 int32 数据 8bit 整数 应当显示 -47483648
        oled_show_uint(0, 5, 4294967295, 10);                                   // 显示 uint32 数据 10bit 整数 应当显示 4294967295
        oled_show_chinese(0, 6, 16, (const uint8 *)oled_16x16_chinese, 4);
        system_delay_ms(1000);

        oled_clear();
        oled_show_wave(0, 0, data, 128, 64, 64, 32);                            // 显示一个三角波形 波形宽度 128 波形最大值 64 显示宽度 64 显示最大值 32
        system_delay_ms(1000);
        oled_clear();
        oled_show_wave(0, 0, data, 128, 64, 128 ,64);                           // 显示一个三角波形 波形宽度 128 波形最大值 64 显示宽度 128 显示最大值 64
        system_delay_ms(1000);

        oled_clear();
        system_delay_ms(1000);
        oled_full(0xFF);
        system_delay_ms(1000);

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
