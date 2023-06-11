# Fastest Car
## 里程碑
:white_check_mark:**按键**(2023-6-9)
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

:heavy_exclamation_mark:**Kalman_Filter**(2023-6-10)
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

## 参考资料
[第十八届全国大学生智能车竞赛竞速比赛规则](https://blog.csdn.net/zhuoqingjoking97298/article/details/127817742)


[遇到困难睡大觉-2022智能车工程](https://gitee.com/zhewana/TroubleSleeping_2022)


[(智能车比赛)基于 ADS 逐飞库 英飞凌 TC264 377系列 的多核使用经验](https://blog.csdn.net/zhou_zhuo/article/details/128751309?csdn_share_tail=%7B%22type%22%3A%22blog%22%2C%22rType%22%3A%22article%22%2C%22rId%22%3A%22128751309%22%2C%22source%22%3A%22zhou_zhuo%22%7D&fromshare=blogdetail)

[第十七届智能车竞赛--极速越野组总结报告](https://blog.csdn.net/m0_46430715/article/details/126709805)

[MM32_CLion](https://github.com/ZhuYanzhen1/MM32_CLion)



