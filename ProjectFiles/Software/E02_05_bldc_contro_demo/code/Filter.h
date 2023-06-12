#ifndef _FILTER_H_
#define _FILTER_H_

#include "zf_common_headfile.h"

typedef struct
{
    float LastP;//上次估算协方差 初始化值为0.02
    float Now_P;//当前估算协方差 初始化值为0
    float out;//卡尔曼滤波器输出 初始化值为0
    float Kg;//卡尔曼增益 初始化值为0
    float Q;//过程噪声协方差 初始化值为0.001
    float R;//观测噪声协方差 初始化值为0.543
}KFP;//Kalman Filter parameter

extern float KalmanFilter(KFP *kfp,float input);
extern void IMU_Filter(void);
extern int IMU_G,IMU_M;
#endif

