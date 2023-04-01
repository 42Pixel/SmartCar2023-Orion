//
// Created by FLY on 2022/5/7.
//
#include "GUI.h"

void GUI_Init(void){
    ips200_init();                                                                     //屏幕初始化 IPS显示范围：0--240，0--320
    ips200_show_string(0,16*0,"Encoder:");												  //
    ips200_show_string(0,16*1,"Motor_Duty:");												//
//    ips200_show_string(0,16*2,"Speed_Target:");											//
    ips200_show_string(0,16*2,"Servo_Duty:");											//

    ips200_draw_line (1,16*3+8,239,16*3+8,RGB565_WHITE);

    ips200_show_string(0, 16*4, "state");                       // 显示定位状态
    ips200_show_string(0, 16*5, "lat_now");                        // 显示现在纬度
    ips200_show_string(0, 16*6, "lon_now");                       // 显示现在经度
    ips200_show_string(0, 16*7, "lat_next");                        // 显示现在纬度
    ips200_show_string(0, 16*8, "lon_next");                       // 显示现在经度
    ips200_show_string(0, 16*9, "GPS_used");                  // 卫星数量
    ips200_show_string(0, 16*10, "direction");                  // 卫星数量
    ips200_show_string(0, 16*11, "Point");                  // 卫星数量


    ips200_draw_line (0,16*12+8,239,16*12+8,RGB565_WHITE);

    ips200_show_string(0, 16*13, "Yaw");                  // Yaw角
    ips200_show_string(0, 16*14, "Dis");             //两点距离
    ips200_show_string(0, 16*15, "Azi");              //方位角
    ips200_show_string(0, 16*16, "Veer");                 //转向角
    ips200_draw_line (0,16*17+8,239,16*17+8,RGB565_WHITE);

    ips200_show_string(0, 16*18, "latitude_get");                  // 踩点经度
    ips200_show_string(0, 16*19, "longitude_get");                  //踩点纬度

 }

void GUI_Show(void){
    ips200_show_int(120,16*0,Encoder_Get,4);
    ips200_show_int(120,16*1,Motor_Duty,4);
//    ips200_show_int(120,16*2,Speed_Target,4);
    ips200_show_int(120, 16*2, Servo_Duty,4);

//    ips200_show_uint(120, 16*5, gps_tau1201.state, 5);								    //定位状态
    ips200_show_float(120, 16*5, gps_tau1201.latitude, 4, 6);				    //当前纬度
    ips200_show_float(120, 16*6, gps_tau1201.longitude, 4, 6);					//当前经度
    ips200_show_float(120, 16*7, flash_union_buffer[point].float_type, 4, 6);				    //下一纬度
    ips200_show_float(120, 16*8, flash_union_buffer[point+50].float_type, 4, 6);					//下一经度
    ips200_show_uint(120, 16*9, gps_tau1201.satellite_used, 5);				            //卫星数量
    ips200_show_float(120, 16*10, gps_tau1201.direction , 4, 6);					//当前经度
    ips200_show_uint(120, 16*11,point  , 2);					//当前经度

    ips200_show_float(120,16*13,eulerAngle.yaw,4,6);
    ips200_show_float(120,16*14,Distance,4,6);
    ips200_show_float(120,16*15,Azimuth,4,6);
    ips200_show_float(120,16*16,Veer,4,6);

    ips200_show_float(120, 16*18, latitude_get, 4, 6);				    //当前纬度
//    ips200_show_float(120, 16*19, longitude_get, 4, 6);					//当前经度
    ips200_show_float(120, 16*19, yaw, 4, 6);					//当前经度
//    ips200_show_int(120, 16*19,fifo_data_count , 4);


}