#include "Motor.h"

#define MAX_DUTY           (30 )                                              // 最大占空比输出限制
#define DIR_CH             (P02_6)                                            // 电机方向输出端口
#define PWM_CH             (ATOM0_CH7_P02_7)                                  // PWM输出端口

#define ENCODER_TIM        (TIM5_ENCODER)                                     // 编码器定时器
#define ENCODER_PLUS       (TIM5_ENCODER_CH1_P10_3)                           // 编码器计数端口
#define ENCODER_DIR        (TIM5_ENCODER_CH2_P10_1)                           // 编码器方向采值端口

#define SERVO_MOTOR_PWM             (ATOM1_CH1_P33_9)                         // 定义主板上舵机对应引脚
#define SERVO_MOTOR_FREQ            (50 )                                     // 定义主板上舵机频率  请务必注意范围 50-300

#define SERVO_MOTOR_L_MAX           (50 )                                     // 定义主板上舵机活动范围 角度
#define SERVO_MOTOR_R_MAX           (150)                                     // 定义主板上舵机活动范围 角度

#define SERVO_MOTOR_DUTY(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*(0.5+(float)(x)/90.0))        //舵机PWM计算

#if (SERVO_MOTOR_FREQ<50 || SERVO_MOTOR_FREQ>300)
    #error "SERVO_MOTOR_FREQ ERROE!"
#endif

float s_pid_KP=0.0;                                                           //舵机PD参数
float s_pid_KD=0.0;

int8 Speed_Duty=20;                                                           // 速度设定值
int16 Encoder;                                                                // 编码器计数



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

    encoder_dir_init(ENCODER_TIM, ENCODER_PLUS, ENCODER_DIR);                  // 初始化编码器采值引脚及定时器

    pwm_init(SERVO_MOTOR_PWM, SERVO_MOTOR_FREQ, 0);                            // 舵机PWM初始化
    pwm_set_duty(SERVO_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY(100.0));            // 舵机回正
}


//----------------------------------------------------------------------------------------------------------------
// 函数简介     读取编码器数值
// 参数说明     encoder            编码器数值
// 返回参数     void
// 使用示例     Encoder_Get();
// 备注信息
//----------------------------------------------------------------------------------------------------------------
void Encoder_Get(void){
    Encoder = encoder_get_count(ENCODER_TIM);                                  // 采集对应编码器数据
    encoder_clear_count(ENCODER_TIM);                                          // 清除对应计数
}


//----------------------------------------------------------------------------------------------------------------
// 函数简介     电机控制
// 参数说明
// 参数说明
// 返回参数
// 使用示例     内部调用
// 备注信息
//----------------------------------------------------------------------------------------------------------------
void Motor_Control(void){
    gpio_set_level(DIR_CH, 0);

    if(Speed_Duty<=0)
       Speed_Duty=0;
    else
        if(Speed_Duty>=MAX_DUTY)
            Speed_Duty=MAX_DUTY;                                                 // 限值Speed_Duty

    pwm_set_duty(PWM_CH, Speed_Duty * (PWM_DUTY_MAX / 100));                     // 计算占空比
}


//----------------------------------------------------------------------------------------------------------------
// 函数简介     舵机PID
// 参数说明
// 输入参数     set                    目标值
// 输入参数     NowPoint               当前值
// 返回参数     output                 PD输出值
// 使用示例     内部调用
// 备注信息
//----------------------------------------------------------------------------------------------------------------
int8 PID_Servo_Contrl(float SetPoint,float NowPoint){
    static float iError,LastError,PrevError;                            //iError:误差,LastError:上次误差，上上次误差
    float output;                                                       //输出
    iError = SetPoint - NowPoint;                                       //当前误差  设定的目标值和实际值的偏差
    output = s_pid_KP * iError+ s_pid_KD * (iError - LastError);        //增量计算

    /*存储误差  用于下次计算*/
    PrevError = LastError;
    LastError = iError;
    return (int)output;                                                 //返回位置值
}


//----------------------------------------------------------------------------------------------------------------
// 函数简介     舵机控制
// 参数说明
// 返回参数
// 使用示例     Servo_Motor_Control();
// 备注信息
//----------------------------------------------------------------------------------------------------------------
void Servo_Motor_Control(void){
    float servo_duty;                                                    // 舵机动作角度
    servo_duty=100+PID_Servo_Contrl(100,101);                            // PD控制
    pwm_set_duty(SERVO_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY(servo_duty)); // 设置舵机PWM
}

