/*********************************************************************************************************************
* TC264 Opensourec Library 即（TC264 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是 TC264 开源库的一部分
*
* TC264 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          gtm_pwm
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          ADS v1.8.0
* 适用平台          TC264D
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2023-02-01       pudding             first version
********************************************************************************************************************/
#ifndef _gtm_pwm_h
#define _gtm_pwm_h

#include "_PinMap/IfxGtm_PinMap.h"
#include "bldc_config.h"
#include "zf_common_typedef.h"

#define MAX_PATTERN 61

#define CMU_CLK_ALL_DISABLE     (0x55555555)    // 时钟全部失能
#define CMU_CLK_ENABLE          (2)             // 时钟使能

#define VALID_IN_PATTERN        (1u)
#define INVALID_IN_PATTERN      (0u)
// TOM输出信号模式选择
#define PAT_PTR_0               (0u)
#define PAT_PTR_1               (1u)
#define PAT_PTR_2               (2u)
#define PAT_PTR_3               (3u)
#define PAT_PTR_4               (4u)
#define PAT_PTR_5               (5u)

// 霍尔硬件顺序    6 2 3 1 5 4
#define HALL_HARDWARE_ORDER_0   (5u)
#define HALL_HARDWARE_ORDER_1   (1u)
#define HALL_HARDWARE_ORDER_2   (3u)
#define HALL_HARDWARE_ORDER_3   (2u)
#define HALL_HARDWARE_ORDER_4   (6u)
#define HALL_HARDWARE_ORDER_5   (4u)
#if BLDC_MOTOR_TYPE == 1
// 电机正转
#define MOTOR_FOREWARD_0        (0xEA8A)    // A上桥C下桥开启，其他关闭
#define MOTOR_FOREWARD_1        (0xE8AA)    // B上桥C下桥开启，其他关闭
#define MOTOR_FOREWARD_2        (0xA8EA)    // B上桥A下桥开启，其他关闭
#define MOTOR_FOREWARD_3        (0x8AEA)    // C上桥A下桥开启，其他关闭
#define MOTOR_FOREWARD_4        (0x8EAA)    // C上桥B下桥开启，其他关闭
#define MOTOR_FOREWARD_5        (0xAE8A)    // A上桥B下桥开启，其他关闭
// 电机反转
#define MOTOR_REVERSAL_0        (0xA8EA)    // B上桥A下桥开启，其他关闭
#define MOTOR_REVERSAL_1        (0x8AEA)    // C上桥A下桥开启，其他关闭
#define MOTOR_REVERSAL_2        (0x8EAA)    // C上桥B下桥开启，其他关闭
#define MOTOR_REVERSAL_3        (0xAE8A)    // A上桥B下桥开启，其他关闭
#define MOTOR_REVERSAL_4        (0xEA8A)    // A上桥C下桥开启，其他关闭
#define MOTOR_REVERSAL_5        (0xE8AA)    // B上桥C下桥开启，其他关闭
#endif
#if BLDC_MOTOR_TYPE == 2
// 电机正转
#define MOTOR_FOREWARD_0        (0xEA8A)    // A上桥C下桥开启，其他关闭
#define MOTOR_FOREWARD_1        (0xE8AA)    // B上桥C下桥开启，其他关闭
#define MOTOR_FOREWARD_2        (0xA8EA)    // B上桥A下桥开启，其他关闭
#define MOTOR_FOREWARD_3        (0x8AEA)    // C上桥A下桥开启，其他关闭
#define MOTOR_FOREWARD_4        (0x8EAA)    // C上桥B下桥开启，其他关闭
#define MOTOR_FOREWARD_5        (0xAE8A)    // A上桥B下桥开启，其他关闭
// 电机反转
#define MOTOR_REVERSAL_0        (0x8AEA)    // C上桥A下桥开启，其他关闭
#define MOTOR_REVERSAL_1        (0x8EAA)    // C上桥B下桥开启，其他关闭
#define MOTOR_REVERSAL_2        (0xAE8A)    // A上桥B下桥开启，其他关闭
#define MOTOR_REVERSAL_3        (0xEA8A)    // A上桥C下桥开启，其他关闭
#define MOTOR_REVERSAL_4        (0xE8AA)    // B上桥C下桥开启，其他关闭
#define MOTOR_REVERSAL_5        (0xA8EA)    // B上桥A下桥开启，其他关闭
#endif
#if BLDC_MOTOR_TYPE == 3
// 电机正转
#define MOTOR_FOREWARD_0        (0xEA8A)    // A上桥C下桥开启，其他关闭
#define MOTOR_FOREWARD_1        (0xE8AA)    // B上桥C下桥开启，其他关闭
#define MOTOR_FOREWARD_2        (0xA8EA)    // B上桥A下桥开启，其他关闭
#define MOTOR_FOREWARD_3        (0x8AEA)    // C上桥A下桥开启，其他关闭
#define MOTOR_FOREWARD_4        (0x8EAA)    // C上桥B下桥开启，其他关闭
#define MOTOR_FOREWARD_5        (0xAE8A)    // A上桥B下桥开启，其他关闭
// 电机反转
#define MOTOR_REVERSAL_0        (0x8EAA)    // C上桥B下桥开启，其他关闭
#define MOTOR_REVERSAL_1        (0xAE8A)    // A上桥B下桥开启，其他关闭
#define MOTOR_REVERSAL_2        (0xEA8A)    // A上桥C下桥开启，其他关闭
#define MOTOR_REVERSAL_3        (0xE8AA)    // B上桥C下桥开启，其他关闭
#define MOTOR_REVERSAL_4        (0xA8EA)    // B上桥A下桥开启，其他关闭
#define MOTOR_REVERSAL_5        (0x8AEA)    // C上桥A下桥开启，其他关闭
#endif

#define MOTOR_BRAKE             (0xEEEA)    // 刹车


#define REV_CNT_VAL             (10u)       // 换相固定次数触发中断
#define FILTER_CON_NUM          (10u)       // 霍尔信号的滤波时长，主要作用是去除毛刺

#define FORCE_UPDATE_EN         (2)         //  输出控制寄存器强制更新
#define FORCE_UPDATE_DIS        (1)         //  忽略更新

#define TOM_OUTPUT_ENABLE       (2)         //  输出通道使能
#define TOM_OUTPUT_DISABLE      (1)         //  输出通道失能

#define ENDIS_EN                (2)         //  更新使能
#define ENDIS_DIS               (1)         //  更新失能

#define TOM_CH_SL_LOW           (0)
#define TOM_CH_SL_HIGH          (1)
#define OSM_EN                  (1)
#define OSM_DIS                 (0)
#define SPEM_EN                 (1)
#define SPEM_DIS                (0)
#define CM0_MATCHING            (0)
#define TRIG_0_MATCHING         (1)
#define TRIG_0_EXT_TRIG         (0)

#define TRIG_CCU0               (1)
#define TRIG_BEFORE             (0)

#define TOM_CH_CMU_FXCLK0       (0)

#define SRC_SPE_INT_PRIO        (90)
#define SRC_PULSE_NOTIFY        (2U)
#define SRC_ENABLE_ALL          (0x1F)
#define SRC_CPU0                (0)
#define SRC_ENABLE              (1)

#define FCY                     ((uint32)100000000)                 // 模块时钟
#define FPWM                    ((uint16)20000)                     // PWM频率
#define PWM_PRIOD_LOAD          (uint16)(5000)                      // PWM周期装载值


// 以下引脚为实际输入输出引脚
#define HALL_A                  (IfxGtm_TIM0_2_TIN33_P33_11_IN )    // 霍尔信号A相输入
#define HALL_B                  (IfxGtm_TIM0_0_TIN34_P33_12_IN )    // 霍尔信号B相输入
#define HALL_C                  (IfxGtm_TIM0_1_TIN35_P33_13_IN )    // 霍尔信号C相输入
#define FSOI_PIN                (IfxGtm_TIM1_6_TIN66_P20_10_IN )    // 刹车信号输入（快速关闭）
#define PWM_PRODUCE_PIN         (IfxGtm_TOM0_0_TOUT26_P33_4_OUT)    // PWM生成引脚
#define A_PHASE_PIN_H           (IfxGtm_TOM0_2_TOUT28_P33_6_OUT)    // 电机A相上桥控制引脚
#define A_PHASE_PIN_L           (IfxGtm_TOM0_3_TOUT29_P33_7_OUT)    // 电机A相下桥控制引脚
#define B_PHASE_PIN_H           (IfxGtm_TOM0_4_TOUT30_P33_8_OUT)    // 电机B相上桥控制引脚
#define B_PHASE_PIN_L           (IfxGtm_TOM0_5_TOUT40_P32_4_OUT)    // 电机B相下桥控制引脚
#define C_PHASE_PIN_H           (IfxGtm_TOM0_6_TOUT42_P23_1_OUT)    // 电机B相上桥控制引脚
#define C_PHASE_PIN_L           (IfxGtm_TOM0_7_TOUT64_P20_8_OUT)    // 电机B相下桥控制引脚


typedef struct
{
    uint32 period;
    uint32 duty;
} speed_st;

typedef struct
{
    uint8 Pattern_AIP[MAX_PATTERN];
    uint8 Pattern_NIP[MAX_PATTERN];
    uint8 Pattern_PIP[MAX_PATTERN];
    uint8 Pattern_PTR[MAX_PATTERN];
    uint8 pattern_cnt;
} pattern_st;

typedef struct
{
    uint32 spe_rcmp; /* SPE revolution counter match event */
    uint32 spe_dchg; /* SPE_DIR bit changed on behalf of new input pattern */
    uint32 spe_perr; /* Wrong or invalid pattern detected at input */
    uint32 spe_bis;  /* Bouncing input signal detected */
    uint32 spe_nipd; /* New input pattern interrupt occurred */
} irq_st;

typedef struct
{
    speed_st    speed;
    pattern_st  pattern;
    pattern_st  pattern_first;
    irq_st      irqcnt;
    uint8       start_pattern;
    uint8       hall_last;
    uint8       hall_now;
} spe_st;


extern spe_st spe_g;

void spe0_pattern_cfg (void);
void pattern_chk(void);
void gtm_bldc_init(void);


#endif

