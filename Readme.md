# Fastest Car
## 任务时间
**2023.3.26----2023.7.25**

## 甘特图

~~~mermaid
gantt
    title Fastest Car
    dateFormat 2023.3.25
    
    section 初期任务
    硬件搭建及测试:       done,des1, 2023-03-25, 11d
    无刷电机闭环控制:   active,des2, after des1, 5d
    姿态传感器解算:            des3, after des2, 5d
    GPS定位:                  des4, after des3, 3d
    转向舵机闭环控制:          des5, after des4, 2d
    GPS寻直线实现:             des6, after des5, 2d
 
    section 中期任务
    GPS循迹测试 :             d1, 2023-04-22, 3d
    GPS绕锥桶&S形:            d2, after d1,   5d

    section 紧急任务(动态更新)
    无刷电机闭环控制:               crit,c1, 2023-04-09, 5d
    
   ~~~




## 任务分配&细节 
**硬件搭建及测试**
- [x] 控制板，驱动板安装，信号线连接
- [x] 转接板固定
- [ ] PCB绘制及制作
- [ ] DEMO下载，硬件测试

**无刷电机闭环控制:**


## 参考资料
[第十八届全国大学生智能车竞赛竞速比赛规则](https://blog.csdn.net/zhuoqingjoking97298/article/details/127817742)


[遇到困难睡大觉-2022智能车工程](https://gitee.com/zhewana/TroubleSleeping_2022)


[(智能车比赛)基于 ADS 逐飞库 英飞凌 TC264 377系列 的多核使用经验](https://blog.csdn.net/zhou_zhuo/article/details/128751309?csdn_share_tail=%7B%22type%22%3A%22blog%22%2C%22rType%22%3A%22article%22%2C%22rId%22%3A%22128751309%22%2C%22source%22%3A%22zhou_zhuo%22%7D&fromshare=blogdetail)

[第十七届智能车竞赛--极速越野组总结报告](https://blog.csdn.net/m0_46430715/article/details/126709805)

[MM32_CLion](https://github.com/ZhuYanzhen1/MM32_CLion)



