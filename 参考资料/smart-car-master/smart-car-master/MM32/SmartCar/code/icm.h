#ifndef _ICM_H
#define _ICM_H
#include "zf_common_headfile.h"

extern int16 icm_gyro_x,icm_gyro_y,icm_gyro_z;
extern int16 icm_acc_x,icm_acc_y,icm_acc_z;

extern struct IMU_Data icmZero;
extern struct IMU_Data icm_data;
extern struct euler_param_t eulerAngle;              // 欧拉角

typedef struct IMU_Data//角加速度输出值
{
    float acc_x;
    float acc_y;
    float acc_z;

    float gyro_x;
    float gyro_y;
    float gyro_z;
}IMU_Data;

typedef struct euler_param_t{
    float pitch;
    float roll;
    float yaw;
}euler_param_t;

//----------数据处理------------
void icmZeroInit(void);     //零漂消除
void icmConvert(void);    //数据单位换算

#endif
