#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "zf_common_headfile.h"


extern float Speed_Target;
extern int16 Encoder_Get;
extern int Motor_Duty;

void Motor_Init(void);
void Get_Encoder (void);
int32 Motor_PID(float Encoder,float Target);
void Motor_Control(void);

#endif