#include "UI.h"

enum UI_Page Page_Num=InfoPage;
/******************************************第一页（通过按键设置是否开跑，点位置零）********************************************/
void UI_Config(void){
    ips200_show_string  (0,     16*1,   "Info-Page");
    ips200_show_string  (0,     16*2,   "latitude");                     ips200_show_float   (120,   16*2,   gps_tau1201.latitude, 4,     6);
    ips200_show_string  (0,     16*3,   "longitude");                    ips200_show_float   (120,   16*3,   gps_tau1201.longitude,4,     6);
    ips200_show_string  (0,     16*4,   "direction");                    ips200_show_float   (120,   16*4,   gps_tau1201.direction,4,     6);
    ips200_show_string  (0,     16*5,   "satellite_used");               ips200_show_uint    (120,   16*5,   gps_tau1201.satellite_used,  5);

    ips200_draw_line    (0,104,239,104,RGB565_WHITE);

    ips200_show_string  (0,     16*7,  "Gyro_Z");                        ips200_show_float   (120,   16*7,   eulerAngle.yaw,3,     3);
    ips200_show_string  (0,     16*8,  "azimuth_N");                     ips200_show_float   (120,   16*8,   azimuth_N,3,     3);
    ips200_show_string  (0,     16*9,  "azimuth_E");                     ips200_show_float   (120,   16*9,   azimuth_E,3,     3);

    ips200_draw_line    (0,168,239,168,RGB565_WHITE);

    ips200_show_string  (0,     16*11,  "GPS_Point");                    ips200_show_uint    (120,     16*11,  GPS_Count,2);
    ips200_show_string  (0,     16*12,  "GPS_Distance");                 ips200_show_float    (120,     16*12,  distance,2,     3);
}

/******************************************第二页（通过按键设置PD参数，各种速度）********************************************/
void UI_Set(void){
    ips200_show_string  (0,     16*1,   "Set-Page");
    ips200_show_string  (0,     16*2,  "Servo_pid_KP");                     ips200_show_float   (120,   16*2,   Servo_pid_KP,4,     6);
    ips200_show_string  (0,     16*3,  "Servo_pid_KD");                     ips200_show_float   (120,   16*3,   Servo_pid_KD,4,     6);
    ips200_show_string  (0,     16*4,  "Speed");                            ips200_show_uint    (120,   16*4,   Start_Speed,3);
    ips200_show_string  (0,     16*5,  "Stop Point");                       ips200_show_uint    (120,   16*5,   Stop_Point,2);
    ips200_show_string  (0,     16*6,  "Back Point");                       ips200_show_uint    (120,   16*6,   Back_Point,3);
}

/******************************************第三页（通过按键打点，存点）********************************************/
void UI_GPS_Point(void){
    ips200_show_string  (0,     16*1,   "GPS-Page");                     ips200_show_uint  (120,     16*1,     GPS_Count,2);
    ips200_show_float   (0,   16*2,   run_latitude[0],4,     6);         ips200_show_float   (120,   16*2,   run_longitude[0],4,     6);
    ips200_show_float   (0,   16*3,   run_latitude[1],4,     6);         ips200_show_float   (120,   16*3,   run_longitude[1],4,     6);
    ips200_show_float   (0,   16*4,   run_latitude[2],4,     6);         ips200_show_float   (120,   16*4,   run_longitude[2],4,     6);
    ips200_show_float   (0,   16*5,   run_latitude[3],4,     6);         ips200_show_float   (120,   16*5,   run_longitude[3],4,     6);
    ips200_show_float   (0,   16*6,   run_latitude[4],4,     6);         ips200_show_float   (120,   16*6,   run_longitude[4],4,     6);
    ips200_show_float   (0,   16*7,   run_latitude[5],4,     6);         ips200_show_float   (120,   16*7,   run_longitude[5],4,     6);
    ips200_show_float   (0,   16*8,   run_latitude[6],4,     6);         ips200_show_float   (120,   16*8,   run_longitude[6],4,     6);
    ips200_show_float   (0,   16*9,   run_latitude[7],4,     6);         ips200_show_float   (120,   16*9,   run_longitude[7],4,     6);
    ips200_show_float   (0,   16*10,   run_latitude[8],4,     6);         ips200_show_float   (120,   16*10,   run_longitude[8],4,     6);

}

/******************************************第四页（待开发）********************************************/
void UI_About(){
    ips200_show_string  (0,     16*1,   "About-Page");

}

void UI(void){
    if(Gui_Page_Refersh){
            ips200_clear();
            Gui_Page_Refersh=false;
        }
    if(Gui_Status){
        switch(Page_Num){
            case InfoPage:
               UI_Config();
                break;
            case GPSPage:
               UI_GPS_Point();
                break;
            case SetPage:
               UI_Set();
                break;
            case AboutPage:
                UI_About();
                break;
        }
    }
}


