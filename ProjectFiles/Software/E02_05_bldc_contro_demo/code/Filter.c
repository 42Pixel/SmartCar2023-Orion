# include "Filter.h"

int IMU_G_X=0, IMU_G_Y=0, IMU_G_Z=0;
int IMU_A_X=0, IMU_A_Y=0, IMU_A_Z=0;
float cpm_k = 0.85;             //二阶滤波系数
float cpm_angle;                //二阶互补滤波滤波值
KFP KFP_IMU_G={0.02,0,0,0,0.001,0.543};
KFP KFP_IMU_M={0.02,0,0,0,0.001,0.543};


/*******************************************************************************************************************************
// 函数简介     卡尔曼滤波
// 参数说明     Kalman *kfp 卡尔曼结构体参数
// 输入参数     float input 需要滤波的参数的测量值（即传感器的采集值）
// 返回参数     滤波后的参数（最优值）
// 使用示例
// 备注信息
********************************************************************************************************************************/
float KalmanFilter(KFP *kfp,float input){
     //预测协方差方程：k时刻系统估算协方差 = k-1时刻的系统协方差 + 过程噪声协方差
     kfp->Now_P = kfp->LastP + kfp->Q;
     //卡尔曼增益方程：卡尔曼增益 = k时刻系统估算协方差 / （k时刻系统估算协方差 + 观测噪声协方差）
     kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
     //更新最优值方程：k时刻状态变量的最优值 = 状态变量的预测值 + 卡尔曼增益 * （测量值 - 状态变量的预测值）
     kfp->out = kfp->out + kfp->Kg * (input -kfp->out);//因为这一次的预测值就是上一次的输出值
     //更新协方差方程: 本次的系统协方差付给 kfp->LastP 威下一次运算准备。
     kfp->LastP = (1-kfp->Kg) * kfp->Now_P;
     return kfp->out;
 }


//-------------------------------------------------------------------------------------------------------------------
//  @brief      低通滤波
//  @param      待滤波的值
//  @return     滤波值
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
float RCFilter(float value,RC_Filter_pt Filter)
{
    Filter->temp = value;
    Filter->value = (1 - Filter->RC) * Filter->value + Filter->RC * Filter->temp;
    return Filter->value;
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      二阶互补滤波 | yaw角
//  @param      now_angle            应该由加速度计得到angle，再使用陀螺仪进行补偿，但是加速度计得不到yaw角，可能还是得使用磁力计
//  @param      now_rate             陀螺仪得到的角速度
//  @param      dt                   采样时间
//  @return     cpm_angle            滤波值
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
float complementary_filter2(float now_angle, float now_rate, float dt)
{
    float y1=0,x1,x2;
    x1 = (now_angle - cpm_angle) * cpm_k * cpm_k;
    y1 = y1 + x1 * dt;
    x2 = y1 + 2 * cpm_k *(now_angle - cpm_angle) + now_rate;
    cpm_angle = cpm_angle + x2 * dt;
    return cpm_angle;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      输入一个值弹出一个值，所有值取平均 | 滑动滤波
//  @param      待滤波的值
//  @return
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------

float Movingaverage_filter(float value,float Filter_buff[])
{
    int8_t i = 0;//遍历
    float temp = value;
    float Filter_sum = 0;
    Filter_buff[Filter_N] = temp;

    for(i = 0; i < Filter_N; i++)
    {
        Filter_buff[i] = Filter_buff[i+1];      //数据左移
        Filter_sum += Filter_buff[i];
    }
    temp = Filter_sum / Filter_N;
    return temp;
}


/*******************************************************************************************************************************
// 函数简介     姿态传感器卡尔曼滤波
// 参数说明
// 输入参数
// 返回参数     滤波后的参数（最优值）
// 使用示例
// 备注信息     放中断
********************************************************************************************************************************/
void IMU_Filter(void){
    IMU_G_X=KalmanFilter(&KFP_IMU_G,(float)imu963ra_gyro_x);
    IMU_G_Y=KalmanFilter(&KFP_IMU_G,(float)imu963ra_gyro_y);
    IMU_G_Z=KalmanFilter(&KFP_IMU_G,(float)imu963ra_gyro_z);

    IMU_A_X=KalmanFilter(&KFP_IMU_M,(float)imu963ra_acc_x);
    IMU_A_Y=KalmanFilter(&KFP_IMU_M,(float)imu963ra_acc_y);
    IMU_A_Z=KalmanFilter(&KFP_IMU_M,(float)imu963ra_acc_z);
}
