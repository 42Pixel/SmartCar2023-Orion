//
// Created by FLY on 2022/5/13.
//

#ifndef _INS_H
#define _INS_H

#include "zf_common_headfile.h"

extern unsigned int point;

extern float Distance,Azimuth,Veer,Direction;
extern float latitude_get,longitude_get;

void INS(void);
void Point_get(void);
void Point_write(void);

extern float lat_get[50];
extern float Landmark_log[50];
#endif //MM32_INS_H
