//
// Created by FLY on 2022/5/3.
//

#ifndef MM32_SERVO_H
#define MM32_SERVO_H

#include "zf_common_headfile.h"

extern float Servo_Duty,text;

int32 Servo_PID(float Current,float Target);

void Servo(void);
void Servo_Init(void);

#endif //MM32_SERVO_H
