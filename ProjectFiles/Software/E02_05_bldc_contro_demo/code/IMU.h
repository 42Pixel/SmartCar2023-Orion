#ifndef CODE_IMU_H_
#define CODE_IMU_H_

#include "zf_common_headfile.h"
#include "math.h"


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
        float pitch;    //俯仰角
        float roll;     //偏航角
        float yaw;       //翻滚角
} euler_param_t;


typedef struct {
    float Xdata;
    float Ydata;
    float Zdata;
} gyro_param_t;

extern quater_param_t Q_info ;  // 全局四元数
extern euler_param_t eulerAngle;
extern int go_flag;

extern signed short  aacx,aacy,aacz;            //加速度传感器原始数据
extern signed short  gyrox,gyroy,gyroz;         //陀螺仪原始数据



void gyroOffset_init(void);

float fast_sqrt(float x);

void ICM_AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az);

//void ICM_init(void);

void ICM_getValues();

void ICM_getEulerianAngles(void);


//extern signed short  aacx,aacy,aacz;            //加速度传感器原始数据
//extern signed short  gyrox,gyroy,gyroz;         //陀螺仪原始数据
//
//void MPU_Get_Raw_data(int16_t *aacx, int16_t *aacy, int16_t *aacz, int16_t *gyrox, int16_t *gyroy, int16_t *gyroz);
//void Euler(void);
//void Kalman_Filter_X(float Accel,float Gyro);
//void Kalman_Filter_Y(float Accel,float Gyro);
//
//inline void Kalman_Filter_Update(float *Angle_Final, float *Q_bias, float Accel, float Gyro, float dt);




#endif
