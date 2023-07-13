#include "RUN.H"

// 定义卡尔曼滤波器结构体
typedef struct {
    float x;    // 估计值
    float P;    // 估计误差的方差
    float Q;    // 过程噪声的方差
    float R;    // 测量噪声的方差
} KalmanFilter;

float GpsOffset;
float azimuth_N,azimuth_E,direction_N;
float distance;
uint8 RunStart_Status=0;

float run_latitude[5]={32.885948,32.886196,32.886440};
float run_longitude[5]={117.427848,117.427833,117.427803};


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



void Run_Start(void){
    pwm_set_duty(SERVO_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY(100));            //跑一段直线获取稳定的GPS方向值
    Speed_Duty=15;

    for(int i=0;i<20;i++){
        GpsOffset+=gps_tau1201.direction;
        system_delay_ms(100);
    }
    GpsOffset/=20;
    RunStart_Status=1;
}

void Run(void){
    static uint8 count=0;
 // 初始化卡尔曼滤波器
    KalmanFilter filter;
    kalmanFilterInit(&filter, 1.0, 1.0, 0.01, 0.1);
    direction_N= kalmanFilterUpdate(&filter,gps_tau1201.direction);

    azimuth_N= get_two_points_azimuth(run_latitude[count], run_longitude[count], run_latitude[count+1], run_longitude[count+1]);

    distance= get_two_points_distance(gps_tau1201.latitude, gps_tau1201.longitude, run_latitude[count], run_longitude[count]);

    if (azimuth_N > 180) {
        azimuth_N -= 360;
       } else if (azimuth_N < -180) {
           azimuth_N += 360;
       }



    azimuth_E=direction_N-azimuth_N;   //目标航向角减去GPS航向角


    if ((distance <= 1.2))
            {
                count++;
            }
    else if(((azimuth_E >= 60) || (azimuth_E <= -60)) && (distance <= 2))
            {
                count++;
            }
    else if (count==2){
        Speed_Duty=0;
    }

}

