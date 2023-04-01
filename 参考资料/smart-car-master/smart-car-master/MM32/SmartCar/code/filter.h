//
// Created by FLY on 2022/5/7.
//

#ifndef _FILTER_H
#define _FILTER_H

#include <stdint.h>

#define Filter_N 10	//连续获取10个陀螺仪数值

extern float yaw_Filter;

struct RC_Para
{
    float temp;  //暂存值,存储RC_baro
    float value; //滤波值
    float RC;    //低通滤波参数
};
typedef struct RC_Para *RC_Filter_pt;



/***************************************************************************/
float Kalmen_getAngle(float now_angle, float now_rate,float dt);
float complementary_filter2(float now_angle, float now_rate, float dt);
float Movingaverage_filter(float value,float Filter_buff[]);
float RCFilter(float value,RC_Filter_pt Filter);

float averageFilter(float in_data);
//float filter(int16_t data);

//float Movingaverage_filter(float value);

#endif //MM32_FILTER_H
