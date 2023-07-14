#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "zf_common_headfile.h"

#define MAX_DUTY           (30 )                                              // 最大占空比输出限制
#define DIR_CH             (P02_6)                                            // 电机方向输出端口
#define PWM_CH             (ATOM0_CH7_P02_7)                                  // PWM输出端口

#define ENCODER_TIM        (TIM5_ENCODER)                                     // 编码器定时器
#define ENCODER_PLUS       (TIM5_ENCODER_CH1_P10_3)                           // 编码器计数端口
#define ENCODER_DIR        (TIM5_ENCODER_CH2_P10_1)                           // 编码器方向采值端口

#define SERVO_MOTOR_PWM             (ATOM1_CH1_P33_9)                         // 定义主板上舵机对应引脚
#define SERVO_MOTOR_FREQ            (100 )                                    // 定义主板上舵机频率  请务必注意范围 50-300

//#define SERVO_MOTOR_DUTY(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*(0.5+(float)(x)/90.0))
#define SERVO_MOTOR_DUTY(x)           (1000*(0.5+(float)(x)/90.0))

#define SERVO_MOTOR_L_MAX           (70 )                                     // 定义主板上舵机活动范围 角度
#define SERVO_MOTOR_R_MAX           (130)                                     // 定义主板上舵机活动范围 角度

#if (SERVO_MOTOR_FREQ<50 || SERVO_MOTOR_FREQ>300)
    #error "SERVO_MOTOR_FREQ ERROE!"
#endif



extern uint8 Speed_Duty;

extern float Servo_pid_KP;                                                           //舵机PD参数
extern float Servo_pid_KD;
extern  float servo_duty;

extern void Servo_Motor_Control(void);
extern void Encoder_Get(void);
extern void Motor_Init(void);
extern void Motor_Control(void);

#endif
