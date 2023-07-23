# 第十八届大学生智能汽车竞赛——猎户座

## 仓库简介
&emsp;&emsp;该仓库为第十八届大学生智能汽车竞赛安徽赛区**猎户座**参赛队伍项目仓库，保存了目前猎户座的全部软硬件资料。

## 目录结构
```
└─Fastest-Car
│  └─ProjectFiles
│      ├─Appendix
│      ├─Hardware
│      │  ├─无线串口模块
│      │  └─猎户座
│      │      ├─FastestCar-Motor
│      │      ├─FastestCar_Ctl
│      │      └─Library
│      └─Software
```
**[Appendix](https://github.com/42Pixel/Fastest-Car/tree/main/ProjectFiles/Appendix):** 附件文件夹，存放参考原理图，开源库，图片等相关文件

**[Hardware](https://github.com/42Pixel/Fastest-Car/tree/main/ProjectFiles/Hardware):** 硬件文件夹，存放相关硬件资料:

&emsp;&emsp;<font size=2>**[无线串口模块](https://github.com/42Pixel/Fastest-Car/tree/main/ProjectFiles/Hardware/%E6%97%A0%E7%BA%BF%E4%B8%B2%E5%8F%A3%E6%A8%A1%E5%9D%97):** 基于433MHZ模块设计的用于智能车急速越野组户外数据透传使用(该项目利用立创EDA设计，请使用立创EDA导入)</font>

&emsp;&emsp;<font size=2>**[猎户座](https://github.com/42Pixel/Fastest-Car/tree/main/ProjectFiles/Hardware/%E7%8C%8E%E6%88%B7%E5%BA%A7):**  根据开源项目：[逐飞科技基于英飞凌TC264 GTM单元的智能车BLDC开源项目](https://gitee.com/seekfree/TC264_GTM_BLDC_Project.git)修改而成。“[FastestCar-Motor](https://github.com/42Pixel/Fastest-Car/tree/main/ProjectFiles/Hardware/%E7%8C%8E%E6%88%B7%E5%BA%A7/FastestCar-Motor)”为无刷电机驱动部分，“[FastestCar_Ctl](https://github.com/42Pixel/Fastest-Car/tree/main/ProjectFiles/Hardware/%E7%8C%8E%E6%88%B7%E5%BA%A7/FastestCar_Ctl)”为驱动控制部分，“[Library](https://github.com/42Pixel/Fastest-Car/tree/main/ProjectFiles/Hardware/%E7%8C%8E%E6%88%B7%E5%BA%A7/Library)”为元器件封装库</font> 

**[Software](https://github.com/42Pixel/Fastest-Car/tree/main/ProjectFiles/Software):** 软件文件夹，存放程序固件。

## 相关图片


## 注意事项
:one: 由于本人代码水平有限，未能成功参赛，Sorftware文件夹中为未完成内容，无法直接用于参赛。

:two: 无线串口模块采用433MHZ透传模块，该模块并不支持一对一配对绑定，当有多个无线串口使用时会出现数据干扰。

:three: 无线串口模块采用嘉立创EDA设计导出，请使用嘉立创EDA导入打开。

:four: FastestCar-Motor项目中，采用双GND线路，即无刷电机与运放、预驱、状态反馈电路并不共地。在焊接调试时，请注意GND线路连接情况。

:five: FastestCar-Motor项目中，若12V-3V3电路无法输出3V3电压，请将RY8121的前馈电容移除。

## 参考资料
[第十八届全国大学生智能车竞赛竞速比赛规则](https://blog.csdn.net/zhuoqingjoking97298/article/details/127817742)

[遇到困难睡大觉-2022智能车工程](https://gitee.com/zhewana/TroubleSleeping_2022)

[(智能车比赛)基于 ADS 逐飞库 英飞凌 TC264 377系列 的多核使用经验](https://blog.csdn.net/zhou_zhuo/article/details/128751309?csdn_share_tail=%7B%22type%22%3A%22blog%22%2C%22rType%22%3A%22article%22%2C%22rId%22%3A%22128751309%22%2C%22source%22%3A%22zhou_zhuo%22%7D&fromshare=blogdetail)

[第十七届智能车竞赛--极速越野组总结报告](https://blog.csdn.net/m0_46430715/article/details/126709805)

[MM32_CLion](https://github.com/ZhuYanzhen1/MM32_CLion)

[智能车视觉组仓库](https://github.com/0clock/smartcar2022)



