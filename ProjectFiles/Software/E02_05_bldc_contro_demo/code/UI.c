#include "UI.h"

void UI_Page1(void){
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

    ips200_show_float   (120,   16*11,   eulerAngle.pitch,4,     6);
    ips200_show_float   (120,   16*12,   eulerAngle.roll,4,     6);
    ips200_show_float   (120,   16*13,   eulerAngle.yaw,4,     6);

    ips200_show_float   (120,   16*14,   s_pid_KP,4,     6);
    ips200_show_float   (120,   16*15,   s_pid_KD,4,     6);
}


