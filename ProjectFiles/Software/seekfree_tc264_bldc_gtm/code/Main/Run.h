#ifndef _RUN_H_
#define _RUN_H_

#include "zf_common_headfile.h"
#include <stdint.h>
#include <stdio.h>

#define                 Array_SIZE                  (64)

typedef struct
{
    float LastP;//上次估算协方差 初始化值为0.02
    float Now_P;//当前估算协方差 初始化值为0
    float out;//卡尔曼滤波器输出 初始化值为0
    float Kg;//卡尔曼增益 初始化值为0
    float Q;//过程噪声协方差 初始化值为0.001
    float R;//观测噪声协方差 初始化值为0.543
}KFP;//Kalman Filter parameter

extern uint8 GPS_Count;
extern uint8 Stop_Point;
extern uint8 Back_Point;
extern uint32 Start_Speed;

extern float Direction_E;
extern float direction_N;
extern float GpsOffset;
extern float azimuth_E;
extern float azimuth_N;
extern float distance;

extern double run_latitude[Array_SIZE];
extern double run_longitude[Array_SIZE];

extern void Run_Start(void);
extern void Point_Get(void);
extern void Run_Status_Set(void);

#endif
