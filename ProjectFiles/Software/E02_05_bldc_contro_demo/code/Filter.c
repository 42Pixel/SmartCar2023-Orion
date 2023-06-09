# include "zf_common_headfile.h"

typedef struct
{
    float Last_P;//上次估算协方差 不可以为0 ! ! ! ! !
    float Now_P;//当前估算协方差
    float out;//卡尔曼滤波器输出
    float Kg;//卡尔曼增益
    float Q;//过程噪声协方差
    float R;//观测噪声协方差
}Kalman;

Kalman KFP_IMU={1,0,0,0,0,0.01};       //Kalman参数初始化

/*******************************************************************************************************************************
// 函数简介     卡尔曼滤波
// 参数说明     Kalman *kfp 卡尔曼结构体参数
// 输入参数     float input 需要滤波的参数的测量值（即传感器的采集值）
// 返回参数     滤波后的参数（最优值）
// 使用示例     Servo_Motor_Control();
// 备注信息
********************************************************************************************************************************/
float KalmanFilter(Kalman *kfp,float input){

   kfp->Now_P = kfp->Last_P + kfp->Q;                       //预测协方差方程：k时刻系统估算协方差 = k-1时刻的系统协方差 + 过程噪声协方差

   kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);            //卡尔曼增益方程：卡尔曼增益 = k时刻系统估算协方差 / （k时刻系统估算协方差 + 观测噪声协方差）

   kfp->out = kfp->out + kfp->Kg * (input -kfp->out);       //更新最优值方程：k时刻状态变量的最优值 = 状态变量的预测值 + 卡尔曼增益 * （测量值 - 状态变量的预测值）
                                                            //因为这一次的预测值就是上一次的输出值
   kfp->Last_P = (1-kfp->Kg) * kfp->Now_P;                  //更新协方差方程: 本次的系统协方差付给 kfp->LastP 威下一次运算准备。
   return kfp->out;
}




IMU=kalmanFilter(KFP_IMU,imu963ra_gyro_z);
