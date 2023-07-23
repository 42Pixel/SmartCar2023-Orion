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
// 接入GPS TAU1201连接
//      模块管脚            单片机管脚
//      RX                  查看 zf_device_gps_tau1201.h 文件中 GPS_TAU1201_RX 宏定义的引脚 默认 P15_7
//      TX                  查看 zf_device_gps_tau1201.h 文件中 GPS_TAU1201_TX 宏定义的引脚 默认 P15_6
//      GND                 电源地
//      VCC                 5V电源
// 接入1.8寸TFT模块
//      模块管脚            单片机管脚
//      SCL                 查看 zf_device_tft180.h 中 TFT180_SCL_PIN 宏定义 默认 P15_3
//      SDA                 查看 zf_device_tft180.h 中 TFT180_SDA_PIN 宏定义 默认 P15_5
//      RES                 查看 zf_device_tft180.h 中 TFT180_RES_PIN 宏定义 默认 P15_1
//      DC                  查看 zf_device_tft180.h 中 TFT180_DC_PIN  宏定义 默认 P15_0
//      CS                  查看 zf_device_tft180.h 中 TFT180_CS_PIN  宏定义 默认 P15_2
//      BL                  查看 zf_device_tft180.h 中 TFT180_BL_PIN  宏定义 默认 P15_4
//      GND                 核心板电源地 GND
//      3V3                 核心板 3V3 电源



// *************************** 例程测试说明 ***************************
// 1.核心板烧录完成本例程 接好GPS 连接好屏幕
// 2.在断电情况下完成连接 完成上电
// 3.屏幕会显示当前GPS收到的数据解析结果
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查

// **************************** 代码区域 ****************************


int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等

    gps_init();
    tft180_init();

    // 此处编写用户代码 例如外设初始化代码等
	cpu_wait_event_ready();         // 等待所有核心初始化完毕
	while (TRUE)
	{
        // 此处编写需要循环执行的代码

	    //gps数据接收与解析都是通过串口中断调用gps_uart_callback函数进行实现的
        //数据解析完毕之后gps_tau1201_flag标志位会置1
        if(gps_tau1201_flag)
        {
            gps_tau1201_flag = 0;

            gps_data_parse();           //开始解析数据

            tft180_show_uint(  0, 16*0, gps_tau1201.time.year, 4);
            tft180_show_uint( 40, 16*0, gps_tau1201.time.month, 2);
            tft180_show_uint( 80, 16*0, gps_tau1201.time.day, 2);
            tft180_show_uint(  0, 16*1, gps_tau1201.time.hour, 2);
            tft180_show_uint( 40, 16*1, gps_tau1201.time.minute, 2);
            tft180_show_uint( 80, 16*1, gps_tau1201.time.second, 2);

            tft180_show_uint(  0, 16*2, gps_tau1201.state, 5);
            tft180_show_float( 0, 16*3, gps_tau1201.latitude, 4, 6);
            tft180_show_float( 0, 16*4, gps_tau1201.longitude, 4, 6);
            tft180_show_float( 0, 16*5, gps_tau1201.speed, 4, 6);
            tft180_show_float( 0, 16*6, gps_tau1201.direction, 4, 6);
            tft180_show_uint(  0, 16*7, gps_tau1201.satellite_used, 5);
            tft180_show_float( 0, 16*8, gps_tau1201.height, 4, 6);
        }
        system_delay_ms(1000);//这里延时主要目的是为了降低输出速度，便于在串口助手中观察数据，实际使用的时候不需要这样去延时

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
//      检查一下GPS模块接线是否正常
// 问题2：数据显示除了时间以外全部是0
//      需要把天线放到室外 需要接收到卫星信号才能有数据
