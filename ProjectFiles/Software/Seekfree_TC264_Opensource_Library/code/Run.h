#ifndef _RUN_H_
#define _RUN_H_

#include "zf_common_headfile.h"
#include <stdint.h>
#include <stdio.h>


extern uint8 RunStart_Status;
extern float GpsOffset;
extern float azimuth_E,azimuth_N;

extern float run_latitude[5];
extern float run_longitude[5];

extern void Run_Start(void);
extern void Run(void);


#endif
