#include "RUN.H"

// 定义卡尔曼滤波器结构体
typedef struct {
    float x;    // 估计值
    float P;    // 估计误差的方差
    float Q;    // 过程噪声的方差
    float R;    // 测量噪声的方差
} KalmanFilter;

double run_latitude[Array_SIZE];
double run_longitude[Array_SIZE];


float GpsOffset;
float azimuth_N;
float azimuth_E;
float direction_N;
float distance;
float Filter_buff[10] = {0}; // 滤波缓冲区

uint8 GPS_Count=0;
uint8 Start_Speed=15;


// 初始化卡尔曼滤波器
void kalmanFilterInit(KalmanFilter* filter, float initialValue, float initialError, float processNoise, float measurementNoise) {
    filter->x = initialValue;
    filter->P = initialError;
    filter->Q = processNoise;
    filter->R = measurementNoise;
}


// 卡尔曼滤波
double kalmanFilterUpdate(KalmanFilter* filter, double measurement) {
    // 预测步骤
    double x_pred = filter->x;
    double P_pred = filter->P + filter->Q;

    // 更新步骤
    double K = P_pred / (P_pred + filter->R);
    filter->x = x_pred + K * (measurement - x_pred);
    filter->P = (1 - K) * P_pred;

    return filter->x;
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

void Point_Get(void){
    if(GPS_Count<=Array_SIZE-1){
        run_latitude[GPS_Count]=gps_tau1201.latitude;
        run_longitude[GPS_Count]=gps_tau1201.longitude;
        GPS_Count++;
    }
}

void Run_Start(void){
    static double startpoint1[2];
    static double startpoint2[2];
//    if(Run_Start_Status){
        if(Run_Start_Status){
        startpoint1[0] = gps_tau1201.latitude;
        startpoint1[1] = gps_tau1201.longitude;
        pwm_set_duty(SERVO_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY(100));            //跑一段直线获取稳定的GPS方向值
        Speed_Duty=Start_Speed;
        system_delay_ms(3000);
        startpoint2[0] = gps_tau1201.latitude;
        startpoint2[1] = gps_tau1201.longitude;
        GpsOffset=get_two_points_azimuth(startpoint1[0], startpoint1[1], startpoint2[0], startpoint2[1])-eulerAngle.yaw;
        if (GpsOffset > 180) {
            GpsOffset -= 360;
        }
        Run_Start_Status=false;
        Servo_Status=true;                          //解锁舵机控制
        Run_Status=true;
    }
    else{
//        Run
        direction_N = Movingaverage_filter(gps_tau1201.direction, Filter_buff);

        if(GPS_Count<=Array_SIZE-2){
            azimuth_N= get_two_points_azimuth(run_latitude[GPS_Count], run_longitude[GPS_Count], run_latitude[GPS_Count+1], run_longitude[GPS_Count+1]);
                if (azimuth_N > 180) {
                    azimuth_N -= 360;
                } else if (azimuth_N < -180) {
                    azimuth_N += 360;
                }
                azimuth_E=direction_N-azimuth_N-2*GpsOffset;   //目标航向角减去GPS航向角

                distance= get_two_points_distance(gps_tau1201.latitude, gps_tau1201.longitude, run_latitude[GPS_Count+1], run_longitude[GPS_Count+1]);

                if ((distance <= 1.5))
                {
                    GPS_Count++;
                }
                else if(((azimuth_E >= 60) || (azimuth_E <= -60)) && (distance <= 2))
                {
                    GPS_Count++;
                }
        }
        else{
            GPS_Count=Array_SIZE-2;
        }

    }

}

