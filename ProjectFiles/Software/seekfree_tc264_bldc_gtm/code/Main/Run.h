#ifndef _RUN_H_
#define _RUN_H_

#include "zf_common_headfile.h"
#include <stdint.h>
#include <stdio.h>

#define                 Array_SIZE                  (32)
#define Filter_N 10 //连续获取10个陀螺仪数值

extern uint8 GPS_Count;
extern uint8 Start_Speed;
extern float direction_N;
extern float GpsOffset;
extern float azimuth_E;
extern float azimuth_N;
extern float distance;

extern double run_latitude[Array_SIZE];
extern double run_longitude[Array_SIZE];

extern void Run_Start(void);
extern void Point_Get(void);

#endif
