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

// **************************** 代码区域 ****************************


int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
/*********************************************其他外设初始化部分*******************************************************/
    ips200_init(IPS200_TYPE_PARALLEL8);
    MotorCtrl_Init();
    gps_init();
    Key_Init();
    uart_init(UART_2,9600,UART2_TX_P10_5,UART2_RX_P10_6);
    system_delay_ms(500);                  //等待所有硬件初始化完毕
/*********************************************其他外设初始化部分*******************************************************/

/**********************************************定时器初始化部分********************************************************/
    pit_ms_init(CCU60_CH0, 29  );               //按键中断间隔 13毫秒
/**********************************************定时器初始化部分********************************************************/
    cpu_wait_event_ready();         // 等待所有F核心初始化完毕

    VOFA* VOFA_pt = vofa_create();         //创建VOFA对象
                 vofa_init(VOFA_pt,                     //初始化当前的vofa对象
                           vofa_ch_data,ch_sz,
                           custom_buf,custom_sz,
                           cmd_rxbuf,cmd_sz,
                           UART_2,UART_2,UART_2);

    while (TRUE)
    {
        if(gps_tau1201_flag){
            gps_tau1201_flag = 0;
            gps_data_parse();                           // 开始解析数据
        }
        UI();
        Key_Active();
        VOFA_Sent();
        Servo_Motor_Control();
        Run_Start();
        Run_Status_Set();
    }
}

#pragma section all restore
// **************************** 代码区域 ****************************

