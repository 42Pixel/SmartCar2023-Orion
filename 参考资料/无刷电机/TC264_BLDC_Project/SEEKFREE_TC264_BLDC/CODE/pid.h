#ifndef _PID_H
#define _PID_H

#include "common.h"

typedef struct
{
    float kp;           //PI闭环的比例参数
    float ki;           //PI闭环的积分参数
    int32 target_speed; //闭环的目标速度 不要直接修改这个变量
    int32 real_speed;   //当前电机转速
    int32 error;        //目标速度与实际速度误差
    int32 out_max;      //闭环最大输出
    int32 pout;         //闭环输出值
    float iout;         //闭环输出值
    int32 out;          //闭环输出值
}closed_loop_struct;



extern closed_loop_struct closed_loop;
//int16 closed_loop_pi_calc(float pek); //PID运算程序
void closed_loop_pi_init(void); //PI初始化程序
int32 closed_loop_pi_calc(int32 read_error); //PI运算程序
#endif
