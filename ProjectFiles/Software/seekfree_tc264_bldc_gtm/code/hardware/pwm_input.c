/*********************************************************************************************************************
* TC264 Opensourec Library ����TC264 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC264 ��Դ���һ����
*
* TC264 ��Դ�� ���������
* �����Ը���������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù�������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ����֤Э�� ������������Ϊ���İ汾
* ��������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ����֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          pwm_input
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.8.0
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2023-02-01       pudding             first version
********************************************************************************************************************/

#include "ifxGtm_Tim.h"
#include "gtm_pwm.h"
#include "pwm_input.h"

uint16 pwm_in_duty;
IfxGtm_Tim_In driver;


//-------------------------------------------------------------------------------------------------------------------
// �������     ���벶���ʼ��
// ����˵��     move_filter     �ṹ�崫��
// ����˵��     new_data        �����������
// ʹ��ʾ��     move_filter_calc(&speed_filter, 400);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void pwm_input_init(void)
{
    IfxGtm_enable(&MODULE_GTM);
    if(!(MODULE_GTM.CMU.CLK_EN.U & 0x2))
    {
        IfxGtm_Cmu_setClkFrequency(&MODULE_GTM, IfxGtm_Cmu_Clk_1, (float)FCY);
        IfxGtm_Cmu_enableClocks(&MODULE_GTM, IFXGTM_CMU_CLKEN_CLK0);
    }

    IfxGtm_Tim_In_Config config;
    IfxGtm_Tim_In_initConfig(&config, &MODULE_GTM);
    config.timIndex                     = IfxGtm_Tim_2;
    config.channelIndex                 = IfxGtm_Tim_Ch_2;
    config.isrPriority                  = GTM_PWM_IN_PRIORITY;
    config.capture.irqOnNewVal          = TRUE;
    config.capture.irqOnCntOverflow     = TRUE;
    config.timeout.clock                = IfxGtm_Cmu_Clk_1;
    config.filter.inputPin              = &IfxGtm_TIM2_2_TIN70_P20_14_IN;
    config.filter.inputPinMode = IfxPort_InputMode_pullDown;

    IfxGtm_Tim_In_init(&driver, &config);
    driver.periodTick = FPWM;

    gpio_init(MOTOR_DIR_IN_PIN, GPI, 0, GPI_PULL_DOWN);      // ��ʼ��������������
}