#include "attitude_solution.h"
#include <math.h>
#include <stdint.h>
#include "Float.h"

#define delta_T     0.005f  //1ms计算一次
#define M_PI        3.1415926f
#define alpha       0.3f

#define CV_PI       3.1415926535897932384626433832795f

float I_ex, I_ey, I_ez;                         // 误差积分
float Accel_x,Accel_y,Accel_z,Gyro_x,Gyro_y,Gyro_z;
float ax,ay,az;
float Angle_x_temp,Angle_y_temp;
float Angle_X_Final,Angle_Y_Final;

quater_param_t Q_info = {1, 0, 0,0};              // 全局四元数
euler_param_t eulerAngle;                       //欧拉角

icm_param_t icm_data;
gyro_param_t GyroOffset;

float param_Kp = 0.18;                          // 加速度计的收敛速率比例增益
float param_Ki = 0.002;                         // 陀螺仪收敛速率的积分增益 0.004


#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))



float fast_sqrt(float x) {
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long *) &y;
    i = 0x5f3759df - (i >> 1);
    y = *(float *) &i;
    y = y * (1.5f - (halfx * y * y));
    return y;
}

float easy_actan2(float dy, float dx)
{
    float ax = fabs(dx), ay = fabs(dy);
    float temp1 = min(ax, ay)/max(ax, ay);
    float temp2 = temp1*temp1;
    float result = ((-0.0464964749 * temp2 + 0.15931422) * temp2 - 0.327622764) * temp2 * temp1 + temp1;

    if (ay > ax)
    {
        result = (PI / 2) - result;
    }
    if (dx < 0.0f)
    {
        result = PI - result;
    }
    if (dy < 0.0f)
    {
        result = (-result);
    }
    return result;
}


static const float atan2_p1 = 0.9997878412794807f*(float)(180/CV_PI);
static const float atan2_p3 = -0.3258083974640975f*(float)(180/CV_PI);
static const float atan2_p5 = 0.1555786518463281f*(float)(180/CV_PI);
static const float atan2_p7 = -0.04432655554792128f*(float)(180/CV_PI);

float fastAtan2( float y, float x )
{
    float ax = abs(x);
    float ay =abs(y);//首先不分象限，求得一个锐角角度
    float a, c, c2;
    if( ax >= ay )
    {
        c = ay/(ax + (float)DBL_EPSILON);
        c2 = c*c;
        a = (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
    }
    else
    {
        c = ax/(ay + (float)DBL_EPSILON);
        c2 = c*c;
        a = 90.f - (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
    }
    if( x < 0 )//锐角求出后，根据x和y的正负性确定向量的方向，即角度。
        a = 180.f - a;
    if( y < 0 )
        a = 360.f - a;
    return a;
}


void gyroOffset_init(void)                       // 陀螺仪零飘初始化
{
    GyroOffset.Xdata = 0;
    GyroOffset.Ydata = 0;
    GyroOffset.Zdata = 0;
    for (uint16_t i = 0; i < 100; ++i) {
        imu963ra_get_gyro();                    // 获取陀螺仪角速度
        imu963ra_get_acc();                     // 获取加速度计加速度
        GyroOffset.Xdata += imu963ra_gyro_x;
        GyroOffset.Ydata += imu963ra_gyro_y;
        GyroOffset.Zdata += imu963ra_gyro_z;
        system_delay_ms(5);
    }

    GyroOffset.Xdata /= 100;
    GyroOffset.Ydata /= 100;
    GyroOffset.Zdata /= 100;
}




//转化为实际物理值
void ICM_getValues() {
    icm_data.acc_x = (((float) imu963ra_acc_x) * alpha) * 8 / 4098 + icm_data.acc_x * (1 - alpha);
    icm_data.acc_y = (((float) imu963ra_acc_y) * alpha) * 8 / 4098 + icm_data.acc_y * (1 - alpha);
    icm_data.acc_z = (((float) imu963ra_acc_z) * alpha) * 8 / 4098 + icm_data.acc_z * (1 - alpha);

    //陀螺仪角度转弧度
    icm_data.gyro_x = ((float) imu963ra_gyro_x - GyroOffset.Xdata) * M_PI /180 /  14.3f;
    icm_data.gyro_y = ((float) imu963ra_gyro_y - GyroOffset.Ydata) * M_PI /180 /  14.3f;
    icm_data.gyro_z = ((float) imu963ra_gyro_z - GyroOffset.Zdata) * M_PI /180 /  14.3f;
}


void ICM_AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az) {
    float halfT = 0.5 * delta_T;
    float vx, vy, vz;    //当前的机体坐标系上的重力单位向量
    float ex, ey, ez;    //四元数计算值与加速度计测量值的误差
    float q0 = Q_info.q0;
    float q1 = Q_info.q1;
    float q2 = Q_info.q2;
    float q3 = Q_info.q3;
    float q0q0 = q0 * q0;
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q1q1 = q1 * q1;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q3q3 = q3 * q3;

    //对加速度数据进行归一化 得到单位加速度
    float norm = fast_sqrt(ax * ax + ay * ay + az * az);
    ax = ax * norm;
    ay = ay * norm;
    az = az * norm;

    //根据当前四元数的姿态值来估算出各重力分量。用于和加速计实际测量出来的各重力分量进行对比，从而实现对四轴姿态的修正
    vx = 2 * (q1q3 - q0q2);
    vy = 2 * (q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;


    //叉积来计算估算的重力和实际测量的重力这两个重力向量之间的误差。
    ex = ay * vz - az * vy;
    ey = az * vx - ax * vz;
    ez = ax * vy - ay * vx;

    //用叉乘误差来做PI修正陀螺零偏，
    //通过调节 param_Kp，param_Ki 两个参数，
    //可以控制加速度计修正陀螺仪积分姿态的速度。
    I_ex += halfT * ex;   // integral error scaled by Ki
    I_ey += halfT * ey;
    I_ez += halfT * ez;

    gx = gx + param_Kp * ex + param_Ki * I_ex;
    gy = gy + param_Kp * ey + param_Ki * I_ey;
    gz = gz + param_Kp * ez + param_Ki * I_ez;


    /*数据修正完成，下面是四元数微分方程*/


    //四元数微分方程，其中halfT为测量周期的1/2，gx gy gz为陀螺仪角速度，以下都是已知量，这里使用了一阶龙哥库塔求解四元数微分方程
    q0 = q0 + (-q1 * gx - q2 * gy - q3 * gz) * halfT;
    q1 = q1 + (q0 * gx + q2 * gz - q3 * gy) * halfT;
    q2 = q2 + (q0 * gy - q1 * gz + q3 * gx) * halfT;
    q3 = q3 + (q0 * gz + q1 * gy - q2 * gx) * halfT;


    // normalise quaternion
    norm = fast_sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    Q_info.q0 = q0 * norm;
    Q_info.q1 = q1 * norm;
    Q_info.q2 = q2 * norm;
    Q_info.q3 = q3 * norm;
}

/*把四元数转换成欧拉角*/
void ICM_getEulerianAngles(void) {
    imu963ra_get_gyro();    // 获取陀螺仪角速度
    imu963ra_get_acc(); // 获取加速度计加速度

    ICM_getValues();
    ICM_AHRSupdate(icm_data.gyro_x, icm_data.gyro_y, icm_data.gyro_z, icm_data.acc_x, icm_data.acc_y, icm_data.acc_z);

    float q0 = Q_info.q0;
    float q1 = Q_info.q1;
    float q2 = Q_info.q2;
    float q3 = Q_info.q3;

    eulerAngle.pitch = asin(-2 * q1 * q3 + 2 * q0 * q2)*90;                                          // pitch
    eulerAngle.roll  = atan2(1-2 * q1 * q1 - 2 * q2 * q2 , 2 * q2 * q3 + 2 * q0 * q1);               // roll
//    eulerAngle.yaw   = atan2(1-2 * q2 * q2 - 2 * q3 * q3 , 2 * q1 * q2 + 2 * q0 * q3);              // yaw
    eulerAngle.yaw   = fastAtan2( 2 * q1 * q2 + 2 * q0 * q3 , 1-2 * q2 * q2 - 2 * q3 * q3);              // yaw

/*   姿态限制*/
//    if (eulerAngle.roll > 90 || eulerAngle.roll < -90) {
//        if (eulerAngle.pitch > 0) {
//            eulerAngle.pitch = 180 - eulerAngle.pitch;
//        }
//        if (eulerAngle.pitch < 0) {
//            eulerAngle.pitch = -(180 + eulerAngle.pitch);
//        }
//    }
//
//    if (eulerAngle.yaw > 360) {
//        eulerAngle.yaw -= 360;
//    } else if (eulerAngle.yaw < 0) {
//        eulerAngle.yaw += 360;
//    }
}
