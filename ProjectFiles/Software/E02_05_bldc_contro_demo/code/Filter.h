#ifndef _FILTER_H_
#define _FILTER_H_

#include "zf_common_headfile.h"
#include <stdint.h>

#define Filter_N 10 //连续获取10个陀螺仪数值

typedef struct
{
    float LastP;                //上次估算协方差 初始化值为0.02
    float Now_P;                //当前估算协方差 初始化值为0
    float out;                  //卡尔曼滤波器输出 初始化值为0
    float Kg;                   //卡尔曼增益 初始化值为0
    float Q;                    //过程噪声协方差 初始化值为0.001
    float R;                    //观测噪声协方差 初始化值为0.543
}KFP;                           //Kalman Filter parameter

struct RC_Para
{
    float temp;                 //暂存值,存储RC_baro
    float value;                //滤波值
    float RC;                   //低通滤波参数
};
typedef struct RC_Para *RC_Filter_pt;



float Kalmen_getAngle(float now_angle, float now_rate,float dt);
float complementary_filter2(float now_angle, float now_rate, float dt);
float Movingaverage_filter(float value,float Filter_buff[]);
float RCFilter(float value,RC_Filter_pt Filter);


extern float KalmanFilter(KFP *kfp,float input);
extern void IMU_Filter(void);
extern int IMU_G,IMU_M;
extern float yaw_Filter;

#endif

