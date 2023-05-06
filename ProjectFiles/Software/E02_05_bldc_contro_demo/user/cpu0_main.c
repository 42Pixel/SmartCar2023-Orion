#include "zf_common_headfile.h"
#include "isr_config.h"
#pragma section all "cpu0_dsram"

//********************************************************宏定义***********************************************************************
#define IPS200_TYPE          (IPS200_TYPE_PARALLEL8)                            // 双排排针 并口两寸屏 这里宏定义填写 IPS200_TYPE_PARALLEL8


//********************************************************宏定义***********************************************************************

//******************************************************** 变量 ***********************************************************************


//******************************************************** 变量 ***********************************************************************

//********************************************************主函数***********************************************************************
int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口

    gps_init();
    key_init(10);
    Motor_Init();

    ips200_set_dir(IPS200_PORTAIT);
    ips200_set_color(RGB565_WHITE, RGB565_BLACK);
    ips200_init(IPS200_TYPE);


    pit_ms_init(CCU60_CH0, 100);


	cpu_wait_event_ready();         // 等待所有核心初始化完毕
	while (TRUE)
	{

	    ips200_show_int (0,16*7,Encoder_Get(),6);
	    ips200_show_int (0,16*8,key_get_state(KEY_1),6);

	    if(gps_tau1201_flag)
	            {
	                gps_tau1201_flag = 0;

	                gps_data_parse();           //开始解析数据

	                ips200_show_uint(   0, 16*0, gps_tau1201.time.year, 4);
	                ips200_show_uint(  80, 16*0, gps_tau1201.time.month, 2);
	                ips200_show_uint( 160, 16*0, gps_tau1201.time.day, 2);
	                ips200_show_uint(   0, 16*1, gps_tau1201.time.hour, 2);
	                ips200_show_uint(  80, 16*1, gps_tau1201.time.minute, 2);
	                ips200_show_uint( 160, 16*1, gps_tau1201.time.second, 2);

	                ips200_show_uint(   0, 16*2, gps_tau1201.state, 5);
	                ips200_show_float(120, 16*2, gps_tau1201.latitude, 4, 6);
	                ips200_show_float(  0, 16*3, gps_tau1201.longitude, 4, 6);
	                ips200_show_float(120, 16*3, gps_tau1201.speed, 4, 6);
	                ips200_show_float(  0, 16*4, gps_tau1201.direction, 4, 6);
	                ips200_show_uint( 120, 16*4, gps_tau1201.satellite_used, 5);
	                ips200_show_float(  0, 16*5, gps_tau1201.height, 4, 6);
	            }
	}
}
//********************************************************主函数***********************************************************************


#pragma section all restore
// **************************** 代码区域 ****************************
