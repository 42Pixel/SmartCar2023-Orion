#include "zf_common_headfile.h"
#include "isr_config.h"
#pragma section all "cpu0_dsram"

//********************************************************宏定义***********************************************************************
#define IPS200_TYPE          (IPS200_TYPE_PARALLEL8)                            // 双排排针 并口两寸屏 这里宏定义填写 IPS200_TYPE_PARALLEL8

//********************************************************宏定义***********************************************************************


//******************************************************** 变量 ***********************************************************************

//******************************************************** 变量 ***********************************************************************


//********************************************************主函数***********************************************************************
int core0_main(void){
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口

    Motor_Init();
    gps_init();

    ips200_set_dir(IPS200_PORTAIT);
    ips200_set_color(RGB565_WHITE, RGB565_BLACK);
    ips200_init(IPS200_TYPE);

    while(1){
        if(imu963ra_init()){
            ips200_show_string(0,16*0,"IMU Init Erro");                                  // IMU963RA 初始化失败
        }
        else{
           break;
        }
    }

    pit_ms_init(CCU60_CH0, 2);      //IMU中断间隔 毫秒
    pit_ms_init(CCU60_CH1, 5);      //电机中断间隔 毫秒

	cpu_wait_event_ready();         // 等待所有核心初始化完毕

	while (TRUE)
//******************************************************** 注意 ***********************************************************************
	    //不要忘记修改舵机PD控制的参数！！！！
	    //不要忘记修改舵机PD控制的参数！！！！
	    //不要忘记修改舵机PD控制的参数！！！！
//******************************************************** 注意 ***********************************************************************
	{

	    if(gps_tau1201_flag){
	        gps_tau1201_flag = 0;
	        gps_data_parse();           //开始解析数据
	      }


	    ips200_show_string  (0,     16*0,   "Encoder");
	    ips200_show_int     (120,   16*0,   Encoder,            6);
	    ips200_show_string  (0,     16*1,   "Speed");
	    ips200_show_int     (120,   16*1,   Speed_Duty,         6);

	    ips200_draw_line    (0,16*3,239,16*3,RGB565_WHITE);

	    ips200_show_string  (0,     16*4,   "state");
        ips200_show_uint    (120,   16*4,   gps_tau1201.state,    5);
        ips200_show_string  (0,     16*5,   "latitude");
	    ips200_show_float   (120,   16*5,   gps_tau1201.latitude, 4,     6);
	    ips200_show_string  (0,     16*6,   "longitude");
	    ips200_show_float   (120,   16*6,   gps_tau1201.longitude,4,     6);
	    ips200_show_string  (0,     16*7,   "direction");
	    ips200_show_float   (120,   16*7,   gps_tau1201.direction,4,     6);
	    ips200_show_string  (0,     16*8,   "satellite_used");
	    ips200_show_uint    (120,   16*8,   gps_tau1201.satellite_used,  5);
        ips200_draw_line    (0,16*10,239,16*10,RGB565_WHITE);

        ips200_show_string  (0,     16*11,  "Gyro_Z");
	    ips200_show_int     (120,   16*11,  imu963ra_gyro_z,             5);
        ips200_show_string  (0,     16*12,  "Mag_Z");
        ips200_show_int     (120,   16*12,  imu963ra_mag_z,             5);



	}
}
//********************************************************主函数***********************************************************************


#pragma section all restore
// **************************** 代码区域 ****************************
