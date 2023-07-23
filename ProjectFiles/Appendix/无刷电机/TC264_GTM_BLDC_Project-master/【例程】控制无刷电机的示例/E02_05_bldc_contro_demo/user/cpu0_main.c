#include "zf_common_headfile.h"
#include "isr_config.h"
#pragma section all "cpu0_dsram"

#define IPS200_TYPE          (IPS200_TYPE_PARALLEL8)                            // 双排排针 并口两寸屏 这里宏定义填写 IPS200_TYPE_PARALLEL8

#define MAX_DUTY            (30 )                                              // 最大占空比输出限制
#define DIR_CH1             (P02_6)                                            // 电机方向输出端口
#define PWM_CH1             (ATOM0_CH7_P02_7)                                  // PWM输出端口



#define ENCODER1_TIM        (TIM5_ENCODER)                                     // 编码器定时器
#define ENCODER1_PLUS       (TIM5_ENCODER_CH1_P10_3)                           // 编码器计数端口
#define ENCODER1_DIR        (TIM5_ENCODER_CH2_P10_1)                           // 编码器方向采值端口


int8 duty = 0;                                                                 // 当前占空比
bool dir = true;                                                               // 计数方向
int16 encoder =0;                                                        // 编码器值

int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等

    ips200_set_dir(IPS200_PORTAIT);
    ips200_set_color(RGB565_WHITE, RGB565_BLACK);
    ips200_init(IPS200_TYPE);

    pwm_init(PWM_CH1, 1000, 0);                                                 // PWM 通道1 初始化频率1KHz 占空比初始为0
    gpio_init(DIR_CH1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                          // 初始化电机方向输出引脚
    encoder_dir_init(ENCODER1_TIM, ENCODER1_PLUS, ENCODER1_DIR);                // 初始化编码器采值引脚及定时器



    pit_ms_init(CCU60_CH0, 100);

    // 此处编写用户代码 例如外设初始化代码等
	cpu_wait_event_ready();         // 等待所有核心初始化完毕
	while (TRUE)
	{
        // 此处编写需要循环执行的代码



        ips200_show_int (0,16*0,encoder,6);

        // 此处编写需要循环执行的代码
	}
}

IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);                                                 // 开启中断嵌套
    pit_clear_flag(CCU60_CH0);


    if(duty >= 0)                                                               // 正转
    {
        pwm_set_duty(PWM_CH1, duty * (PWM_DUTY_MAX / 100));                     // 计算占空比
        gpio_set_level(DIR_CH1, 1);

    }
    else                                                                        // 反转
    {
        pwm_set_duty(PWM_CH1, -duty * (PWM_DUTY_MAX / 100));
        gpio_set_level(DIR_CH1, 0);

    }
    if(dir)                                                                     // 根据方向判断计数方向 本例程仅作参考
    {
        duty ++;                                                                // 正向计数
        if(duty >= MAX_DUTY)                                                    // 达到最大值
        dir = false;                                                            // 变更计数方向
    }
    else
    {
        duty --;                                                                // 反向计数
        if(duty <= -MAX_DUTY)                                                   // 达到最小值
        dir = true;                                                             // 变更计数方向
    }

    encoder = encoder_get_count(ENCODER1_TIM);                               // 采集对应编码器数据

    encoder_clear_count(ENCODER1_TIM);                                          // 清除对应计数


}


#pragma section all restore
// **************************** 代码区域 ****************************
// *************************** 例程常见问题说明 ***************************
// 遇到问题时请按照以下问题检查列表检查
// 问题1：串口没有数据
//      查看串口助手打开的是否是正确的串口，检查打开的 COM 口是否对应的是调试下载器或者 USB-TTL 模块的 COM 口
//      如果是使用逐飞科技 英飞凌TriCore 调试下载器连接，那么检查下载器线是否松动，检查核心板串口跳线是否已经焊接，串口跳线查看核心板原理图即可找到
//      如果是使用 USB-TTL 模块连接，那么检查连线是否正常是否松动，模块 TX 是否连接的核心板的 RX，模块 RX 是否连接的核心板的 TX
// 问题2：串口数据乱码
//      查看串口助手设置的波特率是否与程序设置一致，程序中 zf_common_debug.h 文件中 DEBUG_UART_BAUDRATE 宏定义为 debug uart 使用的串口波特率
// 问题3：无刷电机无反应
//      检查信号引脚是否接对，信号线是否松动
//      使用万用表测量实际PWM输出引脚是否有输出或者示波器查看波形
// 问题4：无刷电机转动但转速显示无速度
//      检查信号引脚是否接对，信号线是否松动
//      使用万用表测量对应的无刷电机速度输出是否有信号
