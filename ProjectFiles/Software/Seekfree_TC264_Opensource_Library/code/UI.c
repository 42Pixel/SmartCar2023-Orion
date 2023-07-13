#include "UI.h"

typedef enum
{
    Page1,
    Page2,
    Page3,
    PageNum
}UI_Page;

UI_Page Page[PageNum];

bool Gui_Page_Active=true=false;

void UI_Page1(void){
    ips200_show_string  (0,     16*1,   "latitude");
    ips200_show_float   (120,   16*1,   gps_tau1201.latitude, 4,     6);
    ips200_show_string  (0,     16*2,   "longitude");
    ips200_show_float   (120,   16*2,   gps_tau1201.longitude,4,     6);
    ips200_show_string  (0,     16*3,   "direction");
    ips200_show_float   (120,   16*3,   gps_tau1201.direction,4,     6);
    ips200_show_string  (0,     16*4,   "satellite_used");
    ips200_show_uint    (120,   16*4,   gps_tau1201.satellite_used,  5);
    ips200_draw_line    (0,16*5,239,16*5,RGB565_WHITE);

    ips200_show_string  (0,     16*6,  "Gyro_Z");
    ips200_show_float   (120,   16*6,   eulerAngle.yaw,3,     3);

    ips200_show_string  (0,     16*7,  "azimuth_E");
    ips200_show_float   (120,   16*7,   azimuth_E,3,     3);

    ips200_show_string  (0,     16*8,  "azimuth_N");
    ips200_show_float   (120,   16*8,   azimuth_N,3,     3);

    ips200_show_string  (0,     16*10,  "s_pid_KP");
    ips200_show_float   (120,   16*10,   s_pid_KP,4,     6);
    ips200_show_string  (0,     16*11,  "s_pid_KD");
    ips200_show_float   (120,   16*11,   s_pid_KD,4,     6);

//    ips200_show_string  (0,     16*14,  "flash_data[0]");
//    ips200_show_float   (120,   16*14,   flash_union_buffer[0].float_type,4,     6);
//
//    ips200_show_string  (0,     16*15,  "flash_data[0]");
//    ips200_show_float   (120,   16*14,   flash_union_buffer[1].float_type,4,     6);
}

void UI_Page2(void){
    ips200_show_float   (0,   16*1,   run_latitude[0],4,     6);         ips200_show_float   (120,   16*1,   run_longitude[0],4,     6);
    ips200_show_float   (0,   16*2,   run_latitude[0],4,     6);         ips200_show_float   (120,   16*2,   run_longitude[1],4,     6);
    ips200_show_float   (0,   16*3,   run_latitude[0],4,     6);         ips200_show_float   (120,   16*3,   run_longitude[2],4,     6);

}


void UI(void){
    if(Gui_Page_Active){
            ips200_clear();
            Gui_Page_Active=false;
        }

}


