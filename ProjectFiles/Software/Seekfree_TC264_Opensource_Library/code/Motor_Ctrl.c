#include <Motor_Ctrl.h>

float Servo_pid_KP=2.0;                                                           //舵机PD参数
float Servo_pid_KD=0.2;
uint8 servo_duty;
uint8 Speed_Duty;                                                           // 速度设定值
int16 Encoder;                                                             // 编码器计数

//----------------------------------------------------------------------------------------------------------------
// 函数简介     舵机PID
// 参数说明
// 输入参数     SetPoint               目标值
// 输入参数     NowPoint               当前值
// 返回参数     output                 PD输出值
// 使用示例     内部调用
// 备注信息
//----------------------------------------------------------------------------------------------------------------
int8 PID_Servo(float SetPoint,float NowPoint){
    static float iError,LastError,PrevError;                            //iError:误差,LastError:上次误差，上上次误差
    float output;                                                       //输出
    iError = SetPoint - NowPoint;                                       //当前误差  设定的目标值和实际值的偏差
    output = Servo_pid_KP * iError+ Servo_pid_KD * (iError - LastError);        //增量计算

    /*存储误差  用于下次计算*/
    PrevError = LastError;
    LastError = iError;
    return (int)output;                                                 //返回位置值
}

//----------------------------------------------------------------------------------------------------------------
// 函数简介     电机&编码器初始化
// 参数说明
// 返回参数
// 使用示例     Motor_Init();
// 备注信息
//----------------------------------------------------------------------------------------------------------------
void MotorCtrl_Init(void){
    pwm_init(PWM_CH, 1000, 0);                                                 // PWM 通道1 初始化频率1KHz 占空比初始为0
    gpio_init(DIR_CH, GPO, GPIO_HIGH, GPO_PUSH_PULL);                          // 初始化电机方向输出引脚

    encoder_dir_init(ENCODER_TIM, ENCODER_PLUS, ENCODER_DIR);                  // 初始化编码器采值引脚及定时器

    pwm_init(SERVO_MOTOR_PWM, SERVO_MOTOR_FREQ, 0);                            // 舵机PWM初始化
    pwm_set_duty(SERVO_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY(100));              // 舵机回正
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
// 函数简介     舵机控制
// 参数说明
// 返回参数
// 使用示例     Servo_Motor_Control();
// 备注信息
//----------------------------------------------------------------------------------------------------------------
void Servo_Motor_Control(void){

    if(Servo_Status){
//        float servo_duty;                                                    // 舵机动作角度
        servo_duty=100+PID_Servo(azimuth_E,eulerAngle.yaw);                          // PD控制
        if(servo_duty>=SERVO_MOTOR_R_MAX)
            servo_duty=SERVO_MOTOR_R_MAX;
        else if(servo_duty<=SERVO_MOTOR_L_MAX)
            servo_duty=SERVO_MOTOR_L_MAX;
        pwm_set_duty(SERVO_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY(servo_duty)); // 设置舵机PWM
    }
}

