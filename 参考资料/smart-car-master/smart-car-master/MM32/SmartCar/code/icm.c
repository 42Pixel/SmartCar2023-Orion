#include "icm.h"
//--------------------ȫ�ֱ���---------------------//
euler_param_t eulerAngle;              // ŷ����

struct IMU_Data icmZero={0,0,0,0,0,0};               // ������У׼ֵ
struct IMU_Data icm_data={0,0,0,0,0,0};                 // ICM����ֵ

/***********************************************************************************************************************
*�� �� ��:  �����ʼ��
*����˵��:  ͨ���ɼ�һ���������ֵ�������������ƫ��ֵ
*��    ��:  ��
*�� �� ֵ:  ��
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
        icmZero.acc_x += icm_acc_x;    //ȡ��
        icmZero.acc_y += icm_acc_y;
        icmZero.acc_z += icm_acc_z;
        icmZero.gyro_x += icm_gyro_x;
        icmZero.gyro_y += icm_gyro_y;
        icmZero.gyro_z += icm_gyro_z;
        system_delay_ms(5);
    }
    icmZero.acc_x  /= 100;            //��ƽ��
    icmZero.acc_y  /= 100;
    icmZero.acc_z  /= 100;

    icmZero.gyro_x /= 100;
    icmZero.gyro_y /= 100;
    icmZero.gyro_z /= 100;
}

/***********************************************************************************************************************
*�� �� ��:  ���������ݻ���
*����˵��:  ���ɼ�����ֵת��Ϊʵ������ֵ,
*���ٶȼƳ�ʼ������ -> ������Χ: ��8g        ��Ӧ������: 4096 LSB/g
*�����ǳ�ʼ������   -> ������Χ: ��2000 dps  ��Ӧ������: 16.4 LSB/dps
*gyro = (gyro_val / 16.4) ��/s = ((gyro_val / 16.4) * PI / 180) rad/s
*��    ��:  ��
*�� �� ֵ:  ��
***********************************************************************************************************************/
void icmConvert(void)
{
    float alpha=1;
    //��ȡ˲ʱ����
    icm20602_get_gyro();
    icm20602_get_acc();
    //���ٶȼ�
    icm_data.acc_x = (((float) icm_acc_x)*alpha)/4096 + icm_data.acc_x*(1-alpha);
    icm_data.acc_y = (((float) icm_acc_y)*alpha)/4096 + icm_data.acc_y*(1-alpha);
    icm_data.acc_z = (((float) icm_acc_z)*alpha)/4096 + icm_data.acc_z*(1-alpha);

    //! �����ǽ��ٶȱ���ת��Ϊ�����ƽ��ٶ�: deg/s -> rad/s
    icm_data.gyro_x = ((float) icm_gyro_x - icmZero.gyro_x) * PI / 180 / 16.4f;
    icm_data.gyro_y = ((float) icm_gyro_y - icmZero.gyro_y) * PI / 180 / 16.4f;
    icm_data.gyro_z = ((float) icm_gyro_z - icmZero.gyro_z) * PI / 180 / 16.4f;
}
