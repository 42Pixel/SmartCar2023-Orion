//
// Created by FLY on 2022/5/7.
//

#ifndef _AHRS_H
#define _AHRS_H

#include "zf_common_headfile.h"
#include "math.h"
#define delta_T     0.001f  // 采样周期1ms 即频率1KHZ

typedef struct
{
    float q0;
    float q1;
    float q2;
    float q3;
}Q4_t;	//四元数

typedef struct vectorxyz
{
    float x;
    float y;
    float z;
}vectorxyz;	//自然坐标系矢量结构体

typedef struct
{
    vectorxyz a;
    vectorxyz b;
    vectorxyz c;
}Matrix_t;	//坐标矩阵因子

typedef struct
{
    void (* update)(void);

}AHRS_t;

void icmAHRSupdate();
#endif //MM32_AHRS_H
