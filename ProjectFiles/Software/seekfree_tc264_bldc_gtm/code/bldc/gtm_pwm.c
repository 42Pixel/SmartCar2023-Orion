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

#include "IFXGTM_TIM_IN.h"
#include "ifxGtm_Tim.h"
#include "motor.h"
#include "pwm_input.h"
#include "gtm_pwm.h"

spe_st spe_g;


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GTM模块时钟配置
// 参数说明     void
// 使用示例     clock_cfg();
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static void clock_cfg(void)
{
    IfxGtm_enable(&MODULE_GTM);

    GTM_CMU_CLK_EN.U                    = CMU_CLK_ALL_DISABLE;

    GTM_CMU_GCLK_NUM.B.GCLK_NUM         = 1;                        // 分频系数的分子
    GTM_CMU_GCLK_DEN.B.GCLK_DEN         = 1;                        // 分频系数的分母

    GTM_CMU_CLK_0_CTRL.B.CLK_CNT        = 0;                        // 分频系数
    GTM_CMU_CLK_1_CTRL.B.CLK_CNT        = 0;
    GTM_CMU_FXCLK_CTRL.B.FXCLK_SEL      = 0;                        // 选择FXCLK的时钟来源

    GTM_CMU_CLK_EN.B.EN_CLK0            = CMU_CLK_ENABLE;
    GTM_CMU_CLK_EN.B.EN_CLK1            = CMU_CLK_ENABLE;
    GTM_CMU_CLK_EN.B.EN_FXCLK           = CMU_CLK_ENABLE;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GTM模块中断刷新
// 参数说明     void
// 使用示例     ccm_cfg();
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static void ccm_cfg(void)
{
    GTM_CTRL.B.RF_PROT                  = TRUE;                     // 关闭全局中断 1：关闭
    GTM_CTRL.B.RF_PROT                  = FALSE;                    // 开启全局中断 0：开启
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     初始化霍尔捕获以及刹车捕获
// 参数说明     void
// 使用示例     hall_tim_input_cfg();
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static void hall_tim_input_cfg(void)
{
    IfxGtm_PinMap_setTimTin(&HALL_A, IfxPort_InputMode_pullUp);     // 配置霍尔通道为上拉输入模式
    IfxGtm_PinMap_setTimTin(&HALL_B, IfxPort_InputMode_pullUp);
    IfxGtm_PinMap_setTimTin(&HALL_C, IfxPort_InputMode_pullUp);

    IfxGtm_PinMap_setTimTin(&FSOI_PIN, IfxPort_InputMode_pullDown);

    GTM_TIM0_CH0_CTRL.B.CICTRL = IfxGtm_Tim_Input_currentChannel;   // 使用当前通道
    GTM_TIM0_IN_SRC.B.MODE_0   = 0;
    GTM_TIM0_IN_SRC.B.VAL_0    = 0;

    GTM_TIM0_CH1_CTRL.B.CICTRL = IfxGtm_Tim_Input_currentChannel;   // 使用当前通道
    GTM_TIM0_IN_SRC.B.MODE_1   = 0;
    GTM_TIM0_IN_SRC.B.VAL_1    = 0;

    GTM_TIM0_CH2_CTRL.B.CICTRL = IfxGtm_Tim_Input_currentChannel;   // 使用当前通道
    GTM_TIM0_IN_SRC.B.MODE_2   = 0;
    GTM_TIM0_IN_SRC.B.VAL_2    = 0;


    GTM_TIM0_CH0_CTRL.B.CLK_SEL      = IfxGtm_Cmu_Clk_0;
    GTM_TIM0_CH0_CTRL.B.FLT_CNT_FRQ  = IfxGtm_Cmu_Tim_Filter_Clk_0;                     // 滤波计数频率选择
    GTM_TIM0_CH0_CTRL.B.FLT_EN       = TRUE;                                            // 滤波使能
    GTM_TIM0_CH0_CTRL.B.FLT_MODE_FE  = IfxGtm_Tim_FilterMode_individualDeglitchTime;    // 下降沿滤波模式 立即
    GTM_TIM0_CH0_CTRL.B.FLT_MODE_RE  = IfxGtm_Tim_FilterMode_individualDeglitchTime;    // 上降沿滤波模式 立即
    GTM_TIM0_CH0_CTRL.B.FLT_CTR_FE   = IfxGtm_Tim_FilterCounter_upDown;                 // 需要FLT_MODE_RE设置为1才有效
    GTM_TIM0_CH0_CTRL.B.FLT_CTR_RE   = IfxGtm_Tim_FilterCounter_upDown;                 // 需要FLT_MODE_RE设置为1才有效
    GTM_TIM0_CH0_FLT_FE.B.FLT_FE     = FILTER_CON_NUM;                                  // 设置滤波计数值
    GTM_TIM0_CH0_FLT_RE.B.FLT_RE     = FILTER_CON_NUM;

    GTM_TIM0_CH1_CTRL.B.CLK_SEL      = IfxGtm_Cmu_Clk_0;
    GTM_TIM0_CH1_CTRL.B.FLT_CNT_FRQ  = IfxGtm_Cmu_Tim_Filter_Clk_0;                     // 滤波计数频率选择
    GTM_TIM0_CH1_CTRL.B.FLT_EN       = TRUE;                                            // 滤波使能
    GTM_TIM0_CH1_CTRL.B.FLT_MODE_FE  = IfxGtm_Tim_FilterMode_individualDeglitchTime;    // 下降沿滤波模式 立即
    GTM_TIM0_CH1_CTRL.B.FLT_MODE_RE  = IfxGtm_Tim_FilterMode_individualDeglitchTime;    // 上降沿滤波模式 立即
    GTM_TIM0_CH1_CTRL.B.FLT_CTR_FE   = IfxGtm_Tim_FilterCounter_upDown;                 // 需要FLT_MODE_RE设置为1才有效
    GTM_TIM0_CH1_CTRL.B.FLT_CTR_RE   = IfxGtm_Tim_FilterCounter_upDown;                 // 需要FLT_MODE_RE设置为1才有效
    GTM_TIM0_CH1_FLT_FE.B.FLT_FE     = FILTER_CON_NUM;                                  // 设置滤波计数值
    GTM_TIM0_CH1_FLT_RE.B.FLT_RE     = FILTER_CON_NUM;

    GTM_TIM0_CH2_CTRL.B.CLK_SEL      = IfxGtm_Cmu_Clk_0;
    GTM_TIM0_CH2_CTRL.B.FLT_CNT_FRQ  = IfxGtm_Cmu_Tim_Filter_Clk_0;                     // 滤波计数频率选择
    GTM_TIM0_CH2_CTRL.B.FLT_EN       = TRUE;                                            // 滤波使能
    GTM_TIM0_CH2_CTRL.B.FLT_MODE_FE  = IfxGtm_Tim_FilterMode_individualDeglitchTime;    // 下降沿滤波模式 立即
    GTM_TIM0_CH2_CTRL.B.FLT_MODE_RE  = IfxGtm_Tim_FilterMode_individualDeglitchTime;    // 上降沿滤波模式 立即
    GTM_TIM0_CH2_CTRL.B.FLT_CTR_FE   = IfxGtm_Tim_FilterCounter_upDown;                 // 需要FLT_MODE_RE设置为1才有效
    GTM_TIM0_CH2_CTRL.B.FLT_CTR_RE   = IfxGtm_Tim_FilterCounter_upDown;                 // 需要FLT_MODE_RE设置为1才有效
    GTM_TIM0_CH2_FLT_FE.B.FLT_FE     = FILTER_CON_NUM;                                  // 设置滤波计数值
    GTM_TIM0_CH2_FLT_RE.B.FLT_RE     = FILTER_CON_NUM;

    GTM_TIM0_CH0_CTRL.B.TIM_MODE     = IfxGtm_Tim_Mode_inputEvent;                      // 输入事件
    GTM_TIM0_CH0_CTRL.B.ISL          = TRUE;                                            // 双边沿触发
    GTM_TIM0_CH0_CTRL.B.TIM_EN       = TRUE;                                            // 使能
    GTM_TIM0_CH1_CTRL.B.TIM_MODE     = IfxGtm_Tim_Mode_inputEvent;
    GTM_TIM0_CH1_CTRL.B.ISL          = TRUE;
    GTM_TIM0_CH1_CTRL.B.TIM_EN       = TRUE;
    GTM_TIM0_CH2_CTRL.B.TIM_MODE     = IfxGtm_Tim_Mode_inputEvent;
    GTM_TIM0_CH2_CTRL.B.ISL          = TRUE;
    GTM_TIM0_CH2_CTRL.B.TIM_EN       = TRUE;

    //FSOI引脚，主要用于快速关闭输出
    GTM_TIM1_CH6_CTRL.B.TIM_MODE     = IfxGtm_Tim_Mode_inputEvent;
    GTM_TIM1_CH6_CTRL.B.ISL          = TRUE;
    GTM_TIM1_CH6_CTRL.B.TIM_EN       = TRUE;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPE0模块基本参数配置
// 参数说明     void
// 使用示例     spe0_pattern_init_cfg();
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static void spe0_pattern_init_cfg (void)
{
    spe_g.start_pattern = GTM_SPE0_CTRL_STAT.B.NIP;

    switch (spe_g.start_pattern)
    {
        case HALL_HARDWARE_ORDER_0:
            GTM_SPE0_CTRL_STAT.B.SPE_PAT_PTR        = PAT_PTR_0;
            GTM_SPE0_CTRL_STAT.B.PIP                = HALL_HARDWARE_ORDER_5;
            GTM_SPE0_CTRL_STAT.B.AIP                = HALL_HARDWARE_ORDER_0;
            GTM_SPE0_OUT_CTRL.B.SPE_OUT_CTRL        = MOTOR_FOREWARD_0;
            break;
        case HALL_HARDWARE_ORDER_1:
            GTM_SPE0_CTRL_STAT.B.SPE_PAT_PTR        = PAT_PTR_1;
            GTM_SPE0_CTRL_STAT.B.PIP                = HALL_HARDWARE_ORDER_0;
            GTM_SPE0_CTRL_STAT.B.AIP                = HALL_HARDWARE_ORDER_1;
            GTM_SPE0_OUT_CTRL.B.SPE_OUT_CTRL        = MOTOR_FOREWARD_1;
            break;
        case HALL_HARDWARE_ORDER_2:
            GTM_SPE0_CTRL_STAT.B.SPE_PAT_PTR        = PAT_PTR_2;
            GTM_SPE0_CTRL_STAT.B.PIP                = HALL_HARDWARE_ORDER_1;
            GTM_SPE0_CTRL_STAT.B.AIP                = HALL_HARDWARE_ORDER_2;
            GTM_SPE0_OUT_CTRL.B.SPE_OUT_CTRL        = MOTOR_FOREWARD_2;
            break;
        case HALL_HARDWARE_ORDER_3:
            GTM_SPE0_CTRL_STAT.B.SPE_PAT_PTR        = PAT_PTR_3;
            GTM_SPE0_CTRL_STAT.B.PIP                = HALL_HARDWARE_ORDER_2;
            GTM_SPE0_CTRL_STAT.B.AIP                = HALL_HARDWARE_ORDER_3;
            GTM_SPE0_OUT_CTRL.B.SPE_OUT_CTRL        = MOTOR_FOREWARD_3;
            break;
        case HALL_HARDWARE_ORDER_4:
            GTM_SPE0_CTRL_STAT.B.SPE_PAT_PTR        = PAT_PTR_4;
            GTM_SPE0_CTRL_STAT.B.PIP                = HALL_HARDWARE_ORDER_3;
            GTM_SPE0_CTRL_STAT.B.AIP                = HALL_HARDWARE_ORDER_4;
            GTM_SPE0_OUT_CTRL.B.SPE_OUT_CTRL        = MOTOR_FOREWARD_4;
            break;
        case HALL_HARDWARE_ORDER_5:
            GTM_SPE0_CTRL_STAT.B.SPE_PAT_PTR        = PAT_PTR_5;
            GTM_SPE0_CTRL_STAT.B.PIP                = HALL_HARDWARE_ORDER_4;
            GTM_SPE0_CTRL_STAT.B.AIP                = HALL_HARDWARE_ORDER_5;
            GTM_SPE0_OUT_CTRL.B.SPE_OUT_CTRL        = MOTOR_FOREWARD_5;
            break;
        default:
            break;
    }
    spe_g.pattern_first.Pattern_AIP[spe_g.pattern_first.pattern_cnt] = GTM_SPE0_CTRL_STAT.B.AIP;
    spe_g.pattern_first.Pattern_NIP[spe_g.pattern_first.pattern_cnt] = GTM_SPE0_CTRL_STAT.B.NIP;
    spe_g.pattern_first.Pattern_PIP[spe_g.pattern_first.pattern_cnt] = GTM_SPE0_CTRL_STAT.B.PIP;
    spe_g.pattern_first.Pattern_PTR[spe_g.pattern_first.pattern_cnt] = GTM_SPE0_CTRL_STAT.B.SPE_PAT_PTR;
    spe_g.pattern_first.pattern_cnt ++;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPE0模块参数配置
// 参数说明     void
// 使用示例     clock_cfg();
// 备注信息     用于霍尔检测错误时的配置刷新
//-------------------------------------------------------------------------------------------------------------------
void spe0_pattern_cfg (void)
{
    spe_g.start_pattern = GTM_SPE0_CTRL_STAT.B.NIP;

    switch (spe_g.start_pattern)
    {
        case HALL_HARDWARE_ORDER_0:
            GTM_SPE0_CTRL_STAT.B.SPE_PAT_PTR        = PAT_PTR_0;
            GTM_SPE0_CTRL_STAT.B.PIP                = HALL_HARDWARE_ORDER_5;
            GTM_SPE0_CTRL_STAT.B.AIP                = HALL_HARDWARE_ORDER_0;
            GTM_SPE0_OUT_CTRL.B.SPE_OUT_CTRL        = MOTOR_FOREWARD_0;
            break;
        case HALL_HARDWARE_ORDER_1:
            GTM_SPE0_CTRL_STAT.B.SPE_PAT_PTR        = PAT_PTR_1;
            GTM_SPE0_CTRL_STAT.B.PIP                = HALL_HARDWARE_ORDER_0;
            GTM_SPE0_CTRL_STAT.B.AIP                = HALL_HARDWARE_ORDER_1;
            GTM_SPE0_OUT_CTRL.B.SPE_OUT_CTRL        = MOTOR_FOREWARD_1;
            break;
        case HALL_HARDWARE_ORDER_2:
            GTM_SPE0_CTRL_STAT.B.SPE_PAT_PTR        = PAT_PTR_2;
            GTM_SPE0_CTRL_STAT.B.PIP                = HALL_HARDWARE_ORDER_1;
            GTM_SPE0_CTRL_STAT.B.AIP                = HALL_HARDWARE_ORDER_2;
            GTM_SPE0_OUT_CTRL.B.SPE_OUT_CTRL        = MOTOR_FOREWARD_2;
            break;
        case HALL_HARDWARE_ORDER_3:
            GTM_SPE0_CTRL_STAT.B.SPE_PAT_PTR        = PAT_PTR_3;
            GTM_SPE0_CTRL_STAT.B.PIP                = HALL_HARDWARE_ORDER_2;
            GTM_SPE0_CTRL_STAT.B.AIP                = HALL_HARDWARE_ORDER_3;
            GTM_SPE0_OUT_CTRL.B.SPE_OUT_CTRL        = MOTOR_FOREWARD_3;
            break;
        case HALL_HARDWARE_ORDER_4:
            GTM_SPE0_CTRL_STAT.B.SPE_PAT_PTR        = PAT_PTR_4;
            GTM_SPE0_CTRL_STAT.B.PIP                = HALL_HARDWARE_ORDER_3;
            GTM_SPE0_CTRL_STAT.B.AIP                = HALL_HARDWARE_ORDER_4;
            GTM_SPE0_OUT_CTRL.B.SPE_OUT_CTRL        = MOTOR_FOREWARD_4;
            break;
        case HALL_HARDWARE_ORDER_5:
            GTM_SPE0_CTRL_STAT.B.SPE_PAT_PTR        = PAT_PTR_5;
            GTM_SPE0_CTRL_STAT.B.PIP                = HALL_HARDWARE_ORDER_4;
            GTM_SPE0_CTRL_STAT.B.AIP                = HALL_HARDWARE_ORDER_5;
            GTM_SPE0_OUT_CTRL.B.SPE_OUT_CTRL        = MOTOR_FOREWARD_5;
            break;
        default:
            break;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPE0模块中断配置
// 参数说明     void
// 使用示例     spe0_irq_init();
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static void spe0_irq_init(void)
{
    IfxSrc_init(&SRC_GTMSPE0IRQ, SRC_CPU0, SRC_SPE_INT_PRIO);       // 设置SPE的中断优先级 以及处理中断的CPU
    IfxSrc_enable(&SRC_GTMSPE0IRQ);                                 // 中断使能

    GTM_SPE0_IRQ_EN.U   = SRC_ENABLE_ALL;                           // SPE模块中断使能
    GTM_SPE0_EIRQ_EN.U  = SRC_ENABLE_ALL;                           // 错误中断使能
    GTM_SPE0_IRQ_MODE.U = SRC_PULSE_NOTIFY;                         // 中断类型
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPE0模块各通道初始化
// 参数说明     void
// 使用示例     spe0_cfg();
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static void spe0_cfg (void)
{
    spe0_irq_init();

    GTM_SPE0_PAT.B.IP0_VAL = VALID_IN_PATTERN;      // 设置模式有效
    GTM_SPE0_PAT.B.IP1_VAL = VALID_IN_PATTERN;
    GTM_SPE0_PAT.B.IP2_VAL = VALID_IN_PATTERN;
    GTM_SPE0_PAT.B.IP3_VAL = VALID_IN_PATTERN;
    GTM_SPE0_PAT.B.IP4_VAL = VALID_IN_PATTERN;
    GTM_SPE0_PAT.B.IP5_VAL = VALID_IN_PATTERN;
    GTM_SPE0_PAT.B.IP6_VAL = INVALID_IN_PATTERN;    // 设置模式无效
    GTM_SPE0_PAT.B.IP7_VAL = INVALID_IN_PATTERN;


    GTM_SPE0_PAT.B.IP0_PAT = HALL_HARDWARE_ORDER_0; // 设置TIMx y z输入的组合值  实际就是霍尔的数值
    GTM_SPE0_PAT.B.IP1_PAT = HALL_HARDWARE_ORDER_1;
    GTM_SPE0_PAT.B.IP2_PAT = HALL_HARDWARE_ORDER_2;
    GTM_SPE0_PAT.B.IP3_PAT = HALL_HARDWARE_ORDER_3;
    GTM_SPE0_PAT.B.IP4_PAT = HALL_HARDWARE_ORDER_4;
    GTM_SPE0_PAT.B.IP5_PAT = HALL_HARDWARE_ORDER_5;
    GTM_SPE0_PAT.B.IP6_PAT = 0;//无效值
    GTM_SPE0_PAT.B.IP7_PAT = 0;

    motor_control.set_dir = gpio_get_level(MOTOR_DIR_IN_PIN);

    if(motor_control.set_dir == FORWARD)
    {
        GTM_SPE0_OUT_PAT0.U = MOTOR_FOREWARD_0;     // 正转换相表
        GTM_SPE0_OUT_PAT1.U = MOTOR_FOREWARD_1;
        GTM_SPE0_OUT_PAT2.U = MOTOR_FOREWARD_2;
        GTM_SPE0_OUT_PAT3.U = MOTOR_FOREWARD_3;
        GTM_SPE0_OUT_PAT4.U = MOTOR_FOREWARD_4;
        GTM_SPE0_OUT_PAT5.U = MOTOR_FOREWARD_5;
    }
    else
    {
        GTM_SPE0_OUT_PAT0.U = MOTOR_REVERSAL_0;     // 反转换相表
        GTM_SPE0_OUT_PAT1.U = MOTOR_REVERSAL_1;
        GTM_SPE0_OUT_PAT2.U = MOTOR_REVERSAL_2;
        GTM_SPE0_OUT_PAT3.U = MOTOR_REVERSAL_3;
        GTM_SPE0_OUT_PAT4.U = MOTOR_REVERSAL_4;
        GTM_SPE0_OUT_PAT5.U = MOTOR_REVERSAL_5;
    }
    GTM_SPE0_OUT_PAT6.U = 0x0;                      // 无效值
    GTM_SPE0_OUT_PAT7.U = 0x0;

    // 每换相10次触发中断，主要作用是避免通过一个换相时间去计算速度，从而导致计算的速度是波动的。
    // 速度波动是因为霍尔安装误差导致的换相时长是不一样的。
    GTM_SPE0_CMP.B.CMP = REV_CNT_VAL & 0xFFFFFF;
    spe0_pattern_init_cfg();

    GTM_SPE0_CTRL_STAT.B.SPE_EN   = TRUE;
    GTM_SPE0_CTRL_STAT.B.SIE0     = TRUE;                               // x通道使能
    GTM_SPE0_CTRL_STAT.B.SIE1     = TRUE;                               // y通道使能
    GTM_SPE0_CTRL_STAT.B.SIE2     = TRUE;                               // z通道使能
    GTM_SPE0_CTRL_STAT.B.FSOM     = 1;                                  // 快速关闭模式使能
    GTM_SPE0_CTRL_STAT.B.PDIR     = 0;
    GTM_SPE0_CTRL_STAT.B.ADIR     = 0;
    GTM_SPE0_CTRL_STAT.B.FSOL     = 0xFF;                               // 关闭的时候8个通道的输出值设定
    GTM_SPE0_CTRL_STAT.B.TRIG_SEL = 2;                                  // 使用TOM[i] CH1延时触发换相
    GTM_SPE0_CTRL_STAT.B.TIM_SEL  = 0;                                  // 使用TIM0 通道0-通道2作为SPE输入
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPE0-TOM模块间通道配置
// 参数说明     void
// 使用示例     spe0_tom_tgc_cfg();
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static void spe0_tom_tgc_cfg (void)
{
    GTM_TOM0_TGC0_FUPD_CTRL.B.FUPD_CTRL0 = FORCE_UPDATE_EN  & 0x3;      // 强制更新
    GTM_TOM0_TGC0_FUPD_CTRL.B.FUPD_CTRL2 = FORCE_UPDATE_EN  & 0x3;
    GTM_TOM0_TGC0_FUPD_CTRL.B.FUPD_CTRL1 = FORCE_UPDATE_DIS & 0x3;      // 忽略
    GTM_TOM0_TGC0_FUPD_CTRL.B.FUPD_CTRL3 = FORCE_UPDATE_DIS & 0x3;
    GTM_TOM0_TGC0_FUPD_CTRL.B.FUPD_CTRL4 = FORCE_UPDATE_DIS & 0x3;
    GTM_TOM0_TGC0_FUPD_CTRL.B.FUPD_CTRL5 = FORCE_UPDATE_DIS & 0x3;
    GTM_TOM0_TGC0_FUPD_CTRL.B.FUPD_CTRL6 = FORCE_UPDATE_DIS & 0x3;
    GTM_TOM0_TGC0_FUPD_CTRL.B.FUPD_CTRL7 = FORCE_UPDATE_DIS & 0x3;

    GTM_TOM0_TGC0_OUTEN_CTRL.B.OUTEN_CTRL0 = TOM_OUTPUT_ENABLE & 0x3;   // 在更新触发的时候使能输出
    GTM_TOM0_TGC0_OUTEN_CTRL.B.OUTEN_CTRL1 = TOM_OUTPUT_ENABLE & 0x3;
    GTM_TOM0_TGC0_OUTEN_CTRL.B.OUTEN_CTRL2 = TOM_OUTPUT_ENABLE & 0x3;
    GTM_TOM0_TGC0_OUTEN_CTRL.B.OUTEN_CTRL3 = TOM_OUTPUT_ENABLE & 0x3;
    GTM_TOM0_TGC0_OUTEN_CTRL.B.OUTEN_CTRL4 = TOM_OUTPUT_ENABLE & 0x3;
    GTM_TOM0_TGC0_OUTEN_CTRL.B.OUTEN_CTRL5 = TOM_OUTPUT_ENABLE & 0x3;
    GTM_TOM0_TGC0_OUTEN_CTRL.B.OUTEN_CTRL6 = TOM_OUTPUT_ENABLE & 0x3;
    GTM_TOM0_TGC0_OUTEN_CTRL.B.OUTEN_CTRL7 = TOM_OUTPUT_ENABLE & 0x3;

    GTM_TOM0_TGC0_ENDIS_CTRL.B.ENDIS_CTRL0 = ENDIS_EN;
    GTM_TOM0_TGC0_ENDIS_CTRL.B.ENDIS_CTRL2 = ENDIS_EN;
    GTM_TOM0_TGC0_ENDIS_CTRL.B.ENDIS_CTRL1 = ENDIS_DIS;
    GTM_TOM0_TGC0_ENDIS_CTRL.B.ENDIS_CTRL3 = ENDIS_DIS;
    GTM_TOM0_TGC0_ENDIS_CTRL.B.ENDIS_CTRL4 = ENDIS_DIS;
    GTM_TOM0_TGC0_ENDIS_CTRL.B.ENDIS_CTRL5 = ENDIS_DIS;
    GTM_TOM0_TGC0_ENDIS_CTRL.B.ENDIS_CTRL6 = ENDIS_DIS;
    GTM_TOM0_TGC0_ENDIS_CTRL.B.ENDIS_CTRL7 = ENDIS_DIS;

    GTM_TOM0_TGC0_GLB_CTRL.B.UPEN_CTRL0 = ENDIS_EN  & 0x3;
    GTM_TOM0_TGC0_GLB_CTRL.B.UPEN_CTRL2 = ENDIS_EN  & 0x3;
    GTM_TOM0_TGC0_GLB_CTRL.B.UPEN_CTRL1 = ENDIS_DIS & 0x3;
    GTM_TOM0_TGC0_GLB_CTRL.B.UPEN_CTRL3 = ENDIS_DIS & 0x3;
    GTM_TOM0_TGC0_GLB_CTRL.B.UPEN_CTRL4 = ENDIS_DIS & 0x3;
    GTM_TOM0_TGC0_GLB_CTRL.B.UPEN_CTRL5 = ENDIS_DIS & 0x3;
    GTM_TOM0_TGC0_GLB_CTRL.B.UPEN_CTRL6 = ENDIS_DIS & 0x3;
    GTM_TOM0_TGC0_GLB_CTRL.B.UPEN_CTRL7 = ENDIS_DIS & 0x3;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     初始化TOM0输出模块
// 参数说明     void
// 使用示例     spe0_pwm_tom_cfg();
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static void spe0_pwm_tom_cfg (void)
{
    spe0_tom_tgc_cfg ();

    GTM_TOM0_CH2_CTRL.B.OSM        = 1;                 // 开启关闭one shot mode
    GTM_TOM0_CH2_CTRL.B.SPEM       = 1;
    GTM_TOM0_CH2_CTRL.B.CLK_SRC_SR = TOM_CH_CMU_FXCLK0;
    GTM_TOM0_CH2_SR0.U = 10;
    GTM_TOM0_CH2_SR1.U = 10;                            // 设置延迟换相时间

    GTM_TOM0_CH0_CTRL.B.SL         = TOM_CH_SL_HIGH;    // 占空比表示的是高电平的占比
    GTM_TOM0_CH0_CTRL.B.OSM        = OSM_DIS;           // 关闭one shot mode
    GTM_TOM0_CH0_CTRL.B.SPEM       = SPEM_DIS;
    GTM_TOM0_CH0_CTRL.B.TRIGOUT    = TRIG_CCU0;
    GTM_TOM0_CH0_CTRL.B.CLK_SRC_SR = TOM_CH_CMU_FXCLK0;
    GTM_TOM0_CH0_CTRL.B.RST_CCU0   = CM0_MATCHING;

    spe_g.speed.duty   = 100;
    spe_g.speed.period = 5000;

    GTM_TOM0_CH0_SR1.U = spe_g.speed.duty;
    GTM_TOM0_CH0_SR0.U = spe_g.speed.period;

    GTM_TOM0_CH1_CTRL.B.OSM        = OSM_DIS;
    GTM_TOM0_CH1_CTRL.B.SPEM       = SPEM_EN;
    GTM_TOM0_CH1_CTRL.B.TRIGOUT    = TRIG_CCU0;
    GTM_TOM0_CH1_CTRL.B.CLK_SRC_SR = TOM_CH_CMU_FXCLK0;

    GTM_TOM0_CH3_CTRL.B.OSM        = OSM_DIS;
    GTM_TOM0_CH3_CTRL.B.SPEM       = SPEM_EN;
    GTM_TOM0_CH3_CTRL.B.TRIGOUT    = TRIG_CCU0;
    GTM_TOM0_CH3_CTRL.B.CLK_SRC_SR = TOM_CH_CMU_FXCLK0;

    GTM_TOM0_CH4_CTRL.B.OSM        = OSM_DIS;
    GTM_TOM0_CH4_CTRL.B.SPEM       = SPEM_EN;
    GTM_TOM0_CH4_CTRL.B.TRIGOUT    = TRIG_CCU0;
    GTM_TOM0_CH4_CTRL.B.CLK_SRC_SR = TOM_CH_CMU_FXCLK0;

    GTM_TOM0_CH5_CTRL.B.OSM        = OSM_DIS;
    GTM_TOM0_CH5_CTRL.B.SPEM       = SPEM_EN;
    GTM_TOM0_CH5_CTRL.B.TRIGOUT    = TRIG_CCU0;
    GTM_TOM0_CH5_CTRL.B.CLK_SRC_SR = TOM_CH_CMU_FXCLK0;

    GTM_TOM0_CH6_CTRL.B.OSM        = OSM_DIS;
    GTM_TOM0_CH6_CTRL.B.SPEM       = SPEM_EN;
    GTM_TOM0_CH6_CTRL.B.TRIGOUT    = TRIG_CCU0;
    GTM_TOM0_CH6_CTRL.B.CLK_SRC_SR = TOM_CH_CMU_FXCLK0;

    GTM_TOM0_CH7_CTRL.B.OSM        = OSM_DIS;
    GTM_TOM0_CH7_CTRL.B.SPEM       = SPEM_EN;
    GTM_TOM0_CH7_CTRL.B.TRIGOUT    = TRIG_CCU0;
    GTM_TOM0_CH7_CTRL.B.CLK_SRC_SR = TOM_CH_CMU_FXCLK0;

    GTM_TOM0_TGC0_GLB_CTRL.B.HOST_TRIG = 1;             // 最后开启TOM模块全局触发请求
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     选择最终输出引脚
// 参数说明     void
// 使用示例     spe0_pwm_mux_outport_cfg();
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static void spe0_pwm_mux_outport_cfg(void)
{
    IfxGtm_PinMap_setTomTout(&PWM_PRODUCE_PIN,  IfxPort_OutputMode_pushPull, IfxPort_PadDriver_cmosAutomotiveSpeed1);
    IfxGtm_PinMap_setTomTout(&A_PHASE_PIN_H,    IfxPort_OutputMode_pushPull, IfxPort_PadDriver_cmosAutomotiveSpeed1);
    IfxGtm_PinMap_setTomTout(&A_PHASE_PIN_L,    IfxPort_OutputMode_pushPull, IfxPort_PadDriver_cmosAutomotiveSpeed1);
    IfxGtm_PinMap_setTomTout(&B_PHASE_PIN_H,    IfxPort_OutputMode_pushPull, IfxPort_PadDriver_cmosAutomotiveSpeed1);
    IfxGtm_PinMap_setTomTout(&B_PHASE_PIN_L,    IfxPort_OutputMode_pushPull, IfxPort_PadDriver_cmosAutomotiveSpeed1);
    IfxGtm_PinMap_setTomTout(&C_PHASE_PIN_H,    IfxPort_OutputMode_pushPull, IfxPort_PadDriver_cmosAutomotiveSpeed1);
    IfxGtm_PinMap_setTomTout(&C_PHASE_PIN_L,    IfxPort_OutputMode_pushPull, IfxPort_PadDriver_cmosAutomotiveSpeed1);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GTM中断触发检测
// 参数说明     void
// 使用示例     pattern_chk();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void pattern_chk(void)
{
    uint8 aip, nip, pip, ptr;
    aip = spe_g.pattern.Pattern_AIP[spe_g.pattern.pattern_cnt] = GTM_SPE0_CTRL_STAT.B.AIP;
    nip = spe_g.pattern.Pattern_NIP[spe_g.pattern.pattern_cnt] = GTM_SPE0_CTRL_STAT.B.NIP;
    pip = spe_g.pattern.Pattern_PIP[spe_g.pattern.pattern_cnt] = GTM_SPE0_CTRL_STAT.B.PIP;
    ptr = spe_g.pattern.Pattern_PTR[spe_g.pattern.pattern_cnt] = GTM_SPE0_CTRL_STAT.B.SPE_PAT_PTR;
    spe_g.pattern.pattern_cnt ++;
    if (spe_g.pattern.pattern_cnt >= MAX_PATTERN)
    {
        spe_g.pattern.pattern_cnt = 0;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     GTM模块初始化
// 参数说明     void
// 使用示例     clock_cfg();
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
void gtm_bldc_init(void)
{
    clock_cfg();                // GTM模块时钟配置初始化
    ccm_cfg();                  // GTM中断刷新
    hall_tim_input_cfg();       // 初始化霍尔捕获以及刹车捕获
    spe0_cfg();                 // 初始化SPE0模块通道
    spe0_pwm_tom_cfg();         // 初始化TOM0输出模块
    spe0_pwm_mux_outport_cfg(); // 选择最终输出PWM的引脚
}
