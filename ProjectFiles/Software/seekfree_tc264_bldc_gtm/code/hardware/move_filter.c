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
* 文件名称          move_filter
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

#include "move_filter.h"

move_filter_struct speed_filter;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     滑动平均滤波计算
// 参数说明     move_filter     结构体传参
// 参数说明     new_data        新输入的数据
// 使用示例     move_filter_calc(&speed_filter, 400);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void move_filter_calc(move_filter_struct *move_filter, int32 new_data)
{
    // 加上新的数值 减去最末尾的数值 求得最新的和
    move_filter->data_sum = move_filter->data_sum + new_data - move_filter->data_buffer[move_filter->index];
    // 重新求平均值
    move_filter->data_average = move_filter->data_sum / move_filter->buffer_size;

    // 将数据写入缓冲区
    move_filter->data_buffer[move_filter->index] = new_data;
    move_filter->index ++;
    if(move_filter->buffer_size <= move_filter->index)
    {
        move_filter->index = 0;
    }
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     滑动平均滤波初始化
// 参数说明     move_filter     结构体传参
// 使用示例     move_filter_init(&speed_filter);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void move_filter_init(move_filter_struct *move_filter)
{
    move_filter->data_average   = 0;
    move_filter->data_sum       = 0;
    move_filter->index          = 0;
    //设置缓冲区大小
    move_filter->buffer_size    = MOVE_AVERAGE_SIZE;

    uint8 i;
    for(i = 0; i < move_filter->buffer_size; i ++)
    {
        move_filter->data_buffer[i] = 0;
    }
}






