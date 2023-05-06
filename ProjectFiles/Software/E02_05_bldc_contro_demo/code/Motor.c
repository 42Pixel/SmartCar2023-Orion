#include "Motor.h"

#define MAX_DUTY           (30 )                                              // 最大占空比输出限制
#define DIR_CH             (P02_6)                                            // 电机方向输出端口
#define PWM_CH             (ATOM0_CH7_P02_7)                                  // PWM输出端口

#define ENCODER_TIM        (TIM5_ENCODER)                                     // 编码器定时器
#define ENCODER_PLUS       (TIM5_ENCODER_CH1_P10_3)                           // 编码器计数端口
#define ENCODER_DIR        (TIM5_ENCODER_CH2_P10_1)                           // 编码器方向采值端口

int8 Motor_Duty;
int8 Speed_Target;
int16 encoder;
bool dir = true;



float Position_KP =10;
float Position_KI =0;
float Position_KD =5;


//----------------------------------------------------------------------------------------------------------------
// 函数简介     电机&编码器初始化
// 参数说明
// 返回参数
// 使用示例     Motor_Init();
// 备注信息
//----------------------------------------------------------------------------------------------------------------
void Motor_Init(void){

    pwm_init(PWM_CH, 1000, 0);                                                 // PWM 通道1 初始化频率1KHz 占空比初始为0
    gpio_init(DIR_CH, GPO, GPIO_HIGH, GPO_PUSH_PULL);                          // 初始化电机方向输出引脚
    encoder_dir_init(ENCODER_TIM, ENCODER_PLUS, ENCODER_DIR);                // 初始化编码器采值引脚及定时器
}


//----------------------------------------------------------------------------------------------------------------
// 函数简介     电机PID控制
// 参数说明     Encoder            编码器数值
// 参数说明     Target             目标数值
// 返回参数     duty
// 使用示例     Motor_PID(float Encoder,float Target);
// 备注信息
//----------------------------------------------------------------------------------------------------------------
int Motor_PID(int Encoder,int Target){

        static int duty,Bias,Integral_bias,Last_Bias;
        Bias=Target - Encoder;
        Integral_bias+=Bias;
        duty=Position_KP*Bias+Position_KD*(Bias-Last_Bias);
        Last_Bias=Bias;
        return duty;
 }


//----------------------------------------------------------------------------------------------------------------
// 函数简介     读取编码器数值
// 参数说明     encoder            编码器数值
// 返回参数     encoder
// 使用示例     Encoder_Get();
// 备注信息
//----------------------------------------------------------------------------------------------------------------
int16 Encoder_Get(void){
    encoder = encoder_get_count(ENCODER_TIM);                               // 采集对应编码器数据
    encoder_clear_count(ENCODER_TIM);                                          // 清除对应计数
    return  encoder;
}


//----------------------------------------------------------------------------------------------------------------
// 函数简介     电机控制
// 参数说明     Encoder            编码器数值
// 参数说明     Target             目标数值
// 返回参数     duty
// 使用示例     Motor_PID(float Encoder,float Target);
// 备注信息
//----------------------------------------------------------------------------------------------------------------
void Motor_Control(void){
    Motor_Duty=Motor_PID(Encoder_Get(),Speed_Target);

    pwm_set_duty(PWM_CH, Motor_Duty * (PWM_DUTY_MAX / 100));                     // 计算占空比
    gpio_set_level(DIR_CH, 0);
}



