/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2021,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：联系淘宝客服
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            hall
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ3184284598)
 * @version         查看doc内version文件 版本说明
 * @Software        ADS v1.5.2
 * @Target core     TC264D
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-12-10
 ********************************************************************************************************************/

#include "zf_gpio.h"
#include "gpio.h"
#include "ccu6_pwm.h"
#include "move_filter.h"
#include "motor.h"
#include "SEEKFREE_FUN.h"
#include "hall.h"


uint8 control_hall[6]={6,2,3,1,5,4};        //霍尔时序矫正

#if MOTOR_TYPE == 0
uint8 hall_steps_normal[2][8]={             //正常转动的霍尔时序
        {0,2,3,4,5,6,1,0},                    //0仅用于占位 无作用
        {0,6,1,2,3,4,5,0},
};

uint8 hall_steps_advance[2][8]={            //超前1相换向的霍尔时序
        {0,3,4,5,6,1,2,0},                    //0仅用于占位 无作用
        {0,5,6,1,2,3,4,0},
};
#elif MOTOR_TYPE == 1
uint8 hall_steps_normal[2][8]={             //正常转动的霍尔时序
        {0,2,3,4,5,6,1,0},                    //0仅用于占位 无作用
        {0,5,6,1,2,3,4,0},
};

uint8 hall_steps_advance[2][8]={            //超前1相换向的霍尔时序
        {0,3,4,5,6,1,2,0},                    //0仅用于占位 无作用
        {0,4,5,6,1,2,3,0},
};
#endif

int8 hall_value_now;     //当前霍尔的值
int8 hall_value_test=1;  //当前霍尔的值
int8 hall_value_last;    //上次霍尔的值
int8 next_hall_value;    //下一次霍尔的值
uint8  hall_index = 0;   //换向时间的下标
uint16 commutation_time_save[6] = {COMMUTATION_TIMEOUT, COMMUTATION_TIMEOUT, COMMUTATION_TIMEOUT, COMMUTATION_TIMEOUT, COMMUTATION_TIMEOUT, COMMUTATION_TIMEOUT};  //换相时间 保存6次
uint32 commutation_time_sum = 0;    //六次换相时间总和
uint16 commutation_time = COMMUTATION_TIMEOUT;    //统计本次换相所需时间
uint32 commutation_delay = COMMUTATION_TIMEOUT;   //延迟换相延时时间
//uint16 commutation_delay_ratio = 6; //换相延时时间 = commutation_delay_ratio*commutation_time_sum>>7 数值越小则换相越超前

uint8 motor_state;

void hall_init(void)
{
    gpio_init(HALLA_PIN, GPI, 0, PULLUP);
    gpio_init(HALLB_PIN, GPI, 0, PULLUP);
    gpio_init(HALLC_PIN, GPI, 0, PULLUP);
    read_hall_value();
}

void read_hall_value(void)
{
    uint8 ha, hb, hc;

    ha = gpio_get(HALLA_PIN);
    hb = gpio_get(HALLB_PIN);
    hc = gpio_get(HALLC_PIN);

    hall_value_now = ha + hb*2 + hc*4;

    //霍尔时序转换 将原始的霍尔时序重新排列
    for(int i=0; i<6; i++)
    {
        if(hall_value_now == control_hall[i])
        {
            hall_value_now = i+1;
            break;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      计算当前的速度
//  @param      void
//  @return     void
//  @since
//-------------------------------------------------------------------------------------------------------------------
inline void calc_speed(void)
{
    //转速计算
    uint8 i;
    int32 speed;
    commutation_time_sum = 0;
    for(i=0; i<6; i++)
    {
        commutation_time_sum += commutation_time_save[i];
    }

    //电机转速计算说明
    //2.commutation_time_sum是统计电机换相6次会进入多少次T12周期中断
        //2.1 T12周期中断周期中断的频率为20KHz
        //2.2 函数调用关系ccu6_t12_pwm->scan_hall_status->calc_speed
        //2.3 commutation_time为统计换相时间的变量
    //3.通常电机转速我们都用RPM表示，RPM表示每分钟电机的转速
    //3.电机转一圈需要换相的次数等于 电机极对数*6
    //4.因此电机转速等于60*ADC中断频率/电机极对数/commutation_time_sum，这样可以得到电机每分钟的转速

    speed = PWM_PIT_NUM/POLEPAIRS/commutation_time_sum;

    if(REVERSE == motor_control.dir)//电机反转的时候需要对速度取反
    {
        speed = -speed;
    }
    move_filter_calc(&speed_filter, speed);
}

void scan_hall_status(void)
{
    read_hall_value();      //读取当前霍尔值

    commutation_time++;

    //换相超时保护
    //如果换相超过了250ms则认为出现堵转，应该及时将速度设置为0
    if(commutation_time >= COMMUTATION_TIMEOUT)
    {
        commutation_time = COMMUTATION_TIMEOUT;
        commutation_time_save[0] = COMMUTATION_TIMEOUT;
        commutation_time_save[1] = COMMUTATION_TIMEOUT;
        commutation_time_save[2] = COMMUTATION_TIMEOUT;
        commutation_time_save[3] = COMMUTATION_TIMEOUT;
        commutation_time_save[4] = COMMUTATION_TIMEOUT;
        commutation_time_save[5] = COMMUTATION_TIMEOUT;

        //滑动平均滤波初始化
        move_filter_init(&speed_filter);
        motor_state=MOTOR_STOP;
    }
    else
    {
        motor_state=MOTOR_RUN;
    }

    if(hall_value_now==7 || hall_value_now==0) return;      //霍尔数据读取错误 不执行换相部分

    if( ((hall_value_now != hall_value_last) && !commutation_delay) || !next_hall_value)        //开始换相
    {
        hall_index++;
        if(hall_index >= 6)
        {
            hall_index = 0;
        }
        commutation_time_save[hall_index] = commutation_time;       //速度信息保存

        commutation_time=0;


        //转子速度计算
        calc_speed();
        //设置旋转方向并输出方向
        motor_set_dir();

        if((speed_filter.data_average > 4000) || (speed_filter.data_average < -4000))
        {
            next_hall_value = hall_steps_advance[motor_control.dir][hall_value_now];        //            速度大于一定的时候，霍尔延迟较大
            commutation_delay = limit_ab(commutation_time_save[hall_index]/2,0,DELAY_MAX);//            因此采用超前换相加延时的方式去匹配最佳的换相点
        }
        else
        {
            next_hall_value = hall_steps_normal[motor_control.dir][hall_value_now];// 速度较低无需超前换相
            commutation_delay = 0;
        }
        hall_value_last = hall_value_now;
    }
}
