#include "servo.h"

// *************************** 例程硬件连接说明 ***************************
// 接入舵机
//      模块管脚            单片机管脚
//      PWM                 A15
//      GND                 舵机电源 GND 联通 核心板电源地 GND
//      VCC                 舵机电源输出
// **************************** 代码区域 ****************************
#define SERVO_PWM             TIM2_PWM_CH1_A15                            // 定义主板上舵机对应引脚
#define SERVO_FREQ            50                                          // 定义主板上舵机频率  请务必注意范围 50-300

float Servo_Duty,text;

#define SERVO_L_MAX           65                                          // 定义主板上舵机活动范围 角度
#define SERVO_R_MAX           115                                         // 定义主板上舵机活动范围 角度

float KP =5;
float KD =1 ;


// ------------------ 舵机占空比计算方式 ------------------
//
// 舵机对应的 0-180 活动角度对应 控制脉冲的 0.5ms-2.5ms 高电平
//
// 那么不同频率下的占空比计算方式就是
// PWM_DUTY_MAX/(1000/freq)*(1+Angle/180) 在 50hz 时就是 PWM_DUTY_MAX/(1000/50)*(1+Angle/180)
//
// 那么 100hz 下 90度的打角 即高电平时间1.5ms 计算套用为
	// PWM_DUTY_MAX/(1000/100)*(1+90/180) = PWM_DUTY_MAX/10*1.5
//
// ------------------ 舵机占空比计算方式 ------------------
#define SERVO_DUTY(x)			((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_FREQ)*(0.5+(float)x/360))

#if (SERVO_FREQ<50 || SERVO_FREQ>300)
#error "SERVO_FREQ ERROE!"
#endif

void Servo_Init(){
	pwm_init(SERVO_PWM, SERVO_FREQ, 0);                   								 //舵机PWM初始化
//    if(gps_tau1201.state)
    pwm_set_duty(SERVO_PWM, (uint32)SERVO_DUTY(100));            					 //舵机回正1（100°）
}


void Servo(){
    Servo_Duty=Servo_PID(eulerAngle.yaw,Veer)+95;
	if(Servo_Duty<=5)Servo_Duty=5;																	 		//舵机限幅（0-200°）
	if(Servo_Duty>=195)Servo_Duty=195;
	pwm_set_duty(SERVO_PWM, (uint32)SERVO_DUTY(Servo_Duty));         	    //重新计算舵机旋转角度
}


int32 Servo_PID(float Current,float Target){

    static float duty,Bias,Integral_bias,Last_Bias;
    Bias=Target - Current;
    Integral_bias+=Bias;
    duty=KP*Bias+KD*(Bias-Last_Bias);
    Last_Bias=Bias;
    return duty;
}