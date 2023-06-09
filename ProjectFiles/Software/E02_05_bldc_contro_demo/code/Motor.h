#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "zf_common_headfile.h"

extern int8 Speed_Duty;
extern int16 Encoder;

extern void Servo_Motor_Control(void);
extern void Encoder_Get(void);
extern void Motor_Init(void);
extern void Motor_Control(void);


#endif
