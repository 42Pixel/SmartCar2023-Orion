#ifndef _FILTER_h
#define _FILTER_h

#include <stdint.h>

/***************************************************************************/
float Kalmen_getAngle(float now_angle, float now_rate,float dt);
float complementary_filter2(float now_angle, float now_rate, float dt);
float Movingaverage_filter(float value,float Filter_buff[]);
//float RCFilter(float value,RC_Filter_pt Filter);
int Angel_PID(int NowAngel,int TargetAngel);

#endif
