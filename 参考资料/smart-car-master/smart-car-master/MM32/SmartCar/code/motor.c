#include "motor.h"

#define ENCODER_2                   (TIM4_ENCOEDER)
#define ENCODER_2_LSB               (TIM4_ENCOEDER_CH1_B6)
#define ENCODER_2_DIR               (TIM4_ENCOEDER_CH2_B7)
#define DIR				            A0
#define PWM                         TIM5_PWM_CH2_A1

int16 Encoder_Get;
float Speed_Target;
int Motor_Duty;

float Position_KP =10;
float Position_KI =0;
float Position_KD =5;

void Motor_Init(){
    gpio_init(DIR, GPO, GPIO_HIGH, GPO_PUSH_PULL);                          // GPIO 初始化为输出 默认上拉输出高pwm_init(PWM, 17000, 0);
    pwm_init(PWM,17000,0);                                                  //PWM初始化
	encoder_init_dir(ENCODER_2, ENCODER_2_LSB, ENCODER_2_DIR);						    //编码器初始化
}


void Get_Encoder(void)
{
    Encoder_Get = encoder_get_count(ENCODER_2);                                    	 // 获取编码器计数
    encoder_clear_count(ENCODER_2);                                                // 清空编码器计数
}

void Motor_Control(){
    Motor_Duty=Motor_PID(Encoder_Get,Speed_Target);

    if(Motor_Duty<=0){
        gpio_set(DIR, GPIO_LOW);
        Motor_Duty=-Motor_Duty;
    }else{
        gpio_set(DIR, GPIO_HIGH);
    }

    if(Motor_Duty>=5000)    //pwm超过100%会卡死！！！小于0也会卡死！！！
    {
        Motor_Duty=5000;
    }
    if(Motor_Duty<=0){
        Motor_Duty=0;
    }

    pwm_set_duty(PWM, Motor_Duty);                             // 计算占空比
}


int32 Motor_PID(float Encoder,float Target){

		static float duty,Bias,Integral_bias,Last_Bias;
		Bias=Target - Encoder;
		Integral_bias+=Bias;
		duty=Position_KP*Bias+Position_KD*(Bias-Last_Bias);//
		Last_Bias=Bias;
		return duty;
 }

