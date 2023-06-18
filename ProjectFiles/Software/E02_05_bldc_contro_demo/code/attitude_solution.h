#ifndef __ATTITUDE_H
#define __ATTITUDE_H

#include "zf_common_headfile.h"

typedef struct {
    float gyro_x;
    float gyro_y;
    float gyro_z;
    float acc_x;
    float acc_y;
    float acc_z;
} icm_param_t;


typedef struct {
    float q0;
    float q1;
    float q2;
    float q3;
} quater_param_t;


typedef struct {
    float pitch;    //������
    float roll;     //ƫ����
    float yaw;       //������
} euler_param_t;


typedef struct {
    float Xdata;
    float Ydata;
    float Zdata;
} gyro_param_t;

extern quater_param_t Q_info ;  // ȫ����Ԫ��
extern euler_param_t eulerAngle;
extern icm_param_t icm_data;

void gyroOffset_init(void);
float fast_sqrt(float x);
void ICM_getEulerianAngles(void);

#endif
