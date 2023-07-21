#include "MOTORCTRL.H"

float Servo_pid_KP=2.5;                                                           //舵机PD参数
float Servo_pid_KD=0.2;
uint8 servo_duty;

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
//    motor_control.set_dir == REVERSE;
    motor_control.brake_flag=0;                                                // 关闭刹车
    pwm_init(SERVO_MOTOR_PWM, SERVO_MOTOR_FREQ, 0);                            // 舵机PWM初始化
    pwm_set_duty(SERVO_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY(100));              // 舵机回正
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



