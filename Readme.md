# Fastest Car
## 任务时间：2023.3.26----2023.7.25

## 甘特图

~~~mermaid
gantt
    title Fastest Car
    dateFormat 2023.3.25
    section 初期任务
    硬件搭建及测试:    active,des1, 2023-03-25, 11d
    无刷电机闭环控制:          des2, after des1, 5d
    姿态传感器解算:            des3, after des2, 5d
    GPS定位:                  des4, after des3, 3d
    转向舵机闭环控制:          des5, after des4, 2d
    GPS寻直线实现:             des6, after des5, 2d
 
    section 中期任务
    GPS循迹测试 :             d1, 2023-04-22, 3d
    GPS绕锥桶&S形:            d2, after d1,   5d
~~~

## 各项任务细节 
**硬件搭建及测试**
- [x] 控制板，驱动板安装，信号线连接
- [ ] 转接板固定PCB绘制及制作
- [ ] DEMO下载，硬件测试
**无刷电机闭环控制:**
