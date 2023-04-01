#include "icm.h"
//--------------------全局变量---------------------//
euler_param_t eulerAngle;              // 欧拉角

struct IMU_Data icmZero={0,0,0,0,0,0};               // 陀螺仪校准值
struct IMU_Data icm_data={0,0,0,0,0,0};                 // ICM处理值

/***********************************************************************************************************************
*函 数 名:  较零初始化
*功能说明:  通过采集一定数据求均值计算陀螺仪零点偏移值
*形    参:  无
*返 回 值:  无
***********************************************************************************************************************/
void icmZeroInit(void)
{
    icmZero.acc_x = 0;
    icmZero.acc_y = 0;
    icmZero.acc_z = 0;

    icmZero.gyro_x = 0;
    icmZero.gyro_y = 0;
    icmZero.gyro_z = 0;
    for (uint8_t i = 0; i < 100; ++i)
    {
        icm20602_get_gyro();
        icm20602_get_acc();
        icmZero.acc_x += icm_acc_x;    //取和
        icmZero.acc_y += icm_acc_y;
        icmZero.acc_z += icm_acc_z;
        icmZero.gyro_x += icm_gyro_x;
        icmZero.gyro_y += icm_gyro_y;
        icmZero.gyro_z += icm_gyro_z;
        system_delay_ms(5);
    }
    icmZero.acc_x  /= 100;            //求平均
    icmZero.acc_y  /= 100;
    icmZero.acc_z  /= 100;

    icmZero.gyro_x /= 100;
    icmZero.gyro_y /= 100;
    icmZero.gyro_z /= 100;
}

/***********************************************************************************************************************
*函 数 名:  陀螺仪数据换算
*功能说明:  将采集的数值转化为实际物理值,
*加速度计初始化配置 -> 测量范围: ±8g        对应灵敏度: 4096 LSB/g
*陀螺仪初始化配置   -> 测量范围: ±2000 dps  对应灵敏度: 16.4 LSB/dps
*gyro = (gyro_val / 16.4) °/s = ((gyro_val / 16.4) * PI / 180) rad/s
*形    参:  无
*返 回 值:  无
***********************************************************************************************************************/
void icmConvert(void)
{
    float alpha=1;
    //获取瞬时数据
    icm20602_get_gyro();
    icm20602_get_acc();
    //加速度计
    icm_data.acc_x = (((float) icm_acc_x)*alpha)/4096 + icm_data.acc_x*(1-alpha);
    icm_data.acc_y = (((float) icm_acc_y)*alpha)/4096 + icm_data.acc_y*(1-alpha);
    icm_data.acc_z = (((float) icm_acc_z)*alpha)/4096 + icm_data.acc_z*(1-alpha);

    //! 陀螺仪角速度必须转换为弧度制角速度: deg/s -> rad/s
    icm_data.gyro_x = ((float) icm_gyro_x - icmZero.gyro_x) * PI / 180 / 16.4f;
    icm_data.gyro_y = ((float) icm_gyro_y - icmZero.gyro_y) * PI / 180 / 16.4f;
    icm_data.gyro_z = ((float) icm_gyro_z - icmZero.gyro_z) * PI / 180 / 16.4f;
}
