//
// Created by FLY on 2022/5/13.
//

#include "INS.h"
#include "math.h"

#define FLASH_SECTION_INDEX       (FLASH_SECTION_127)                           // 存储数据用的扇区 倒数第一个扇区
#define FLASH_PAGE_INDEX          (FLASH_PAGE_3)                                // 存储数据用的页码 倒数第一个页码
#define Filter_N 10	//连续获取10个数值


float Distance,Azimuth,Veer,Direction;
unsigned int point,x;
float latitude_get,longitude_get;

float filter(int16_t data){
    static int16_t buf[Filter_N];
    static int16_t index=0,flag=0;
    static float sum=0;

    // 替换之前位置上的数据
    sum += data - buf[index];
    buf[index] = data;
    index++;

    // 控制数据循环放置到缓冲区
    if(index==Filter_N)
    {
        index = 0;
        flag = 1;
    }

    // 如果没有充满缓存区，有几个就取几个的平均
    if(flag==0)		return sum/index;
    else			return sum/Filter_N;
}


void INS(void){
//    flash_buffer_clear();
//    flash_read_page_to_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);           // 将数据从 flash 读取到缓冲区

    flash_union_buffer[12].float_type=32.888624;             flash_union_buffer[57].float_type=117.425659;
    flash_union_buffer[11].float_type=32.888164;             flash_union_buffer[61].float_type=117.425720;
    flash_union_buffer[10].float_type=32.888015;             flash_union_buffer[60].float_type=117.425819;
    flash_union_buffer[9].float_type=32.887935;             flash_union_buffer[59].float_type=117.426155;
    flash_union_buffer[8].float_type=32.888042;             flash_union_buffer[58].float_type=117.426437;
    flash_union_buffer[7].float_type=32.888328;             flash_union_buffer[57].float_type=117.426589;
    flash_union_buffer[6].float_type=32.888729;             flash_union_buffer[56].float_type=117.426551;
    flash_union_buffer[5].float_type=32.889152;             flash_union_buffer[55].float_type=117.426498;
    flash_union_buffer[4].float_type=32.889347;             flash_union_buffer[54].float_type=117.426338;
    flash_union_buffer[3].float_type=32.889385;             flash_union_buffer[53].float_type=117.426055;
    flash_union_buffer[2].float_type=32.889328;             flash_union_buffer[52].float_type=117.425804;
    flash_union_buffer[1].float_type=32.889179;             flash_union_buffer[51].float_type=117.425628;
    flash_union_buffer[0].float_type=32.888624;             flash_union_buffer[50].float_type=117.425659;

    Distance=get_two_points_distance (gps_tau1201.latitude,gps_tau1201.longitude, flash_union_buffer[point].float_type, flash_union_buffer[point+50].float_type);
    Azimuth=get_two_points_azimuth (gps_tau1201.latitude, gps_tau1201.longitude, flash_union_buffer[point].float_type, flash_union_buffer[point+50].float_type);

    if(Azimuth>180)Azimuth=Azimuth-360;
    if(gps_tau1201.direction>180)gps_tau1201.direction=gps_tau1201.direction-360;

    Direction=filter(gps_tau1201.direction);

    Veer= Direction-Azimuth;



//    if(Distance<1.5||fabs(Veer)>100){point++;Beep_flag=1;eulerAngle.yaw=0;}
    if(Distance<2){
        point++;
        Beep_flag=1;
    }

    if(point>11)Speed_Target=0;
}



void Point_get(void){
    float lat_get[50];
    float lon_get[50];

    flash_buffer_clear();
    gps_data_parse();
    Beep_flag=1;
    latitude_get=gps_tau1201.latitude;
    longitude_get=gps_tau1201.longitude;

    lat_get[x]=latitude_get;
    lon_get[x]=longitude_get;
    x++;
}


void Point_write(void){
    if(flash_check(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX))                 // 判断是否有数据
        flash_erase_page(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);              // 擦除这一页

    flash_union_buffer[12].float_type=32.888624;             flash_union_buffer[57].float_type=117.425659;
    flash_union_buffer[11].float_type=32.888164;             flash_union_buffer[61].float_type=117.425720;
    flash_union_buffer[10].float_type=32.888015;             flash_union_buffer[60].float_type=117.425819;
    flash_union_buffer[9].float_type=32.887935;             flash_union_buffer[59].float_type=117.426155;
    flash_union_buffer[8].float_type=32.888042;             flash_union_buffer[58].float_type=117.426437;
    flash_union_buffer[7].float_type=32.888328;             flash_union_buffer[57].float_type=117.426589;
    flash_union_buffer[6].float_type=32.888729;             flash_union_buffer[56].float_type=117.426551;
    flash_union_buffer[5].float_type=32.889152;             flash_union_buffer[55].float_type=117.426498;
    flash_union_buffer[4].float_type=32.889347;             flash_union_buffer[54].float_type=117.426338;
    flash_union_buffer[3].float_type=32.889427;             flash_union_buffer[53].float_type=117.426040;
    flash_union_buffer[2].float_type=32.889373;             flash_union_buffer[52].float_type=117.425750;
    flash_union_buffer[1].float_type=32.889179;             flash_union_buffer[51].float_type=117.425628;
    flash_union_buffer[0].float_type=32.888624;             flash_union_buffer[50].float_type=117.425659;


    flash_write_page_from_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);        // 向指定 Flash 扇区的页码写入缓冲区数据
    flash_buffer_clear();
}
