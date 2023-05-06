#include "zf_common_headfile.h"
#include "isr_config.h"
#pragma section all "cpu0_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中

// 对于TC系列默认是不支持中断嵌套的，希望支持中断嵌套需要在中断内使用 interrupt_global_enable(0); 来开启中断嵌套
// 简单点说实际上进入中断后TC系列的硬件自动调用了 interrupt_global_disable(); 来拒绝响应任何的中断，因此需要我们自己手动调用 interrupt_global_enable(0); 来开启中断的响应。


// **************************** 宏定义 ****************************
#define IPS200_TYPE          (IPS200_TYPE_PARALLEL8)                            // 双排排针 并口两寸屏 这里宏定义填写 IPS200_TYPE_PARALLEL8
                                                                               // 单排排针 SPI 两寸屏 这里宏定义填写 IPS200_TYPE_SPI
#define MAX_DUTY             (30 )                                              // 最大占空比输出限制
#define DIR_CH              (P02_6)                                            // 电机方向输出端口
#define PWM_CH              (ATOM0_CH7_P02_7)                                  // PWM输出端口

#define ENCODER_TIM        (TIM5_ENCODER)                                     // 编码器定时器
#define ENCODER_PLUS       (TIM5_ENCODER_CH1_P10_3)                           // 编码器计数端口
#define ENCODER_DIR        (TIM5_ENCODER_CH2_P10_1)                           // 编码器方向采值端口

// **************************** 宏定义 ****************************


// **************************** 变量定义 ****************************
int8 duty = 0;                                                                 // 当前占空比
bool dir = true;                                                               // 计数方向
int32 encoder = 0;
// **************************** 变量定义 ****************************


// **************************** 代码区域 ****************************
int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
//****************************** 初始化 *****************************

    ips200_set_dir(IPS200_PORTAIT);
    ips200_set_color(RGB565_WHITE, RGB565_BLACK);
    ips200_init(IPS200_TYPE);

    pwm_init(PWM_CH, 1000, 0);                                                 // PWM 通道1 初始化频率1KHz 占空比初始为0
    gpio_init(DIR_CH, GPO, GPIO_HIGH, GPO_PUSH_PULL);                          // 初始化电机方向输出引脚
    encoder_dir_init(ENCODER_TIM, ENCODER_PLUS, ENCODER_DIR);                // 初始化编码器采值引脚及定时器

    pit_ms_init(CCU60_CH0, 100);

//****************************** 初始化 *****************************
    cpu_wait_event_ready();         // 等待所有核心初始化完毕

    while (TRUE)
    {
        // 此处编写需要循环执行的代码
        encoder = encoder_get_count(ENCODER_TIM);                               // 采集对应编码器数据
        encoder_clear_count(ENCODER_TIM);                                          // 清除对应计数
        ips200_show_int (0,16*0,encoder,6);


        // 此处编写需要循环执行的代码
    }
}

IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);                                                 // 开启中断嵌套
    pit_clear_flag(CCU60_CH0);

    pwm_set_duty(PWM_CH, duty * (PWM_DUTY_MAX / 100));                     // 计算占空比
    gpio_set_level(DIR_CH, 1);
}

#pragma section all restore
// **************************** 代码区域 ****************************
