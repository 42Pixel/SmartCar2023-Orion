# Fastest Car
## 里程碑
:white_check_mark:**按一下，再按一下 [按键]**(2023-6-9)
```c
enum Keycode{
    Key1,
    Key2,
    Key3,
    Key4
};

extern void Key_Init(void);
extern void Scan_Key(void);
```

:white_check_mark:**稳住，别浪 [Kalman_Filter]**(2023-6-12)
```c
float KalmanFilter(Kalman *kfp,float input){

   kfp->Now_P = kfp->Last_P + kfp->Q;                       //预测协方差方程：k时刻系统估算协方差 = k-1时刻的系统协方差 + 过程噪声协方差
   kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);            //卡尔曼增益方程：卡尔曼增益 = k时刻系统估算协方差 / （k时刻系统估算协方差 + 观测噪声协方差）
   kfp->out = kfp->out + kfp->Kg * (input -kfp->out);       //更新最优值方程：k时刻状态变量的最优值 = 状态变量的预测值 + 卡尔曼增益 * （测量值 - 状态变量的预测值）
                                                            //因为这一次的预测值就是上一次的输出值
   kfp->Last_P = (1-kfp->Kg) * kfp->Now_P;                  //更新协方差方程: 本次的系统协方差付给 kfp->LastP 威下一次运算准备。
   return kfp->out;
}
```
:white_check_mark:**VOFA+原名伏特加，于2018年10月启动，代码配酒，bug没有 [VOFA+]**(2023-6-14)
```c
// VOFA+
	VOFA* VOFA_pt = vofa_create();       //创建VOFA对象
	vofa_init(VOFA_pt,                   //初始化当前的vofa对象
	    vofa_ch_data,ch_sz,
	    custom_buf,custom_sz,
	    cmd_rxbuf,cmd_sz,
	    UART_2,UART_2,UART_2);
```
:white_check_mark:**找准方向[IMU]**
```c
void ICM_getEulerianAngles(void) {
    //采集陀螺仪数据
    imu963ra_get_gyro();    // 获取陀螺仪角速度
    imu963ra_get_acc(); // 获取加速度计加速度

    ICM_getValues();
    ICM_AHRSupdate(icm_data.gyro_x, icm_data.gyro_y, icm_data.gyro_z, icm_data.acc_x, icm_data.acc_y, icm_data.acc_z);

    float q0 = Q_info.q0;
    float q1 = Q_info.q1;
    float q2 = Q_info.q2;
    float q3 = Q_info.q3;

    eulerAngle.pitch = asin(-2 * q1 * q3 + 2 * q0 * q2)*90;                                          // pitch
    eulerAngle.roll  = atan2(1-2 * q1 * q1 - 2 * q2 * q2 , 2 * (q2 * q3 + q0 * q1))*90;              // roll
    eulerAngle.yaw   = atan2(1-2 * q2 * q2 - 2 * q3 * q3 , 2 * (q1 * q2 - q0 * q3))*90;              // yaw
```

:heavy_exclamation_mark:**蹒跚学步[循迹]**

## 参考资料
[第十八届全国大学生智能车竞赛竞速比赛规则](https://blog.csdn.net/zhuoqingjoking97298/article/details/127817742)


[遇到困难睡大觉-2022智能车工程](https://gitee.com/zhewana/TroubleSleeping_2022)


[(智能车比赛)基于 ADS 逐飞库 英飞凌 TC264 377系列 的多核使用经验](https://blog.csdn.net/zhou_zhuo/article/details/128751309?csdn_share_tail=%7B%22type%22%3A%22blog%22%2C%22rType%22%3A%22article%22%2C%22rId%22%3A%22128751309%22%2C%22source%22%3A%22zhou_zhuo%22%7D&fromshare=blogdetail)

[第十七届智能车竞赛--极速越野组总结报告](https://blog.csdn.net/m0_46430715/article/details/126709805)

[MM32_CLion](https://github.com/ZhuYanzhen1/MM32_CLion)



