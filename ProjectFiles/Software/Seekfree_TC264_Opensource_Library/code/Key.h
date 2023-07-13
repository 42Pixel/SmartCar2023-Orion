#ifndef _KEY_H_
#define _KEY_H_

#include "zf_common_headfile.h"


extern uint8 Run_Status;

typedef struct {
    uint8 key1;
    uint8 key2;
    uint8 key3;
    uint8 key4;
} Key_status;

typedef struct {
    uint8 key1;
    uint8 key2;
    uint8 key3;
    uint8 key4;
} Last_status;

typedef struct {
    uint8 key1;
    uint8 key2;
    uint8 key3;
    uint8 key4;
} Key_flag;

extern void Key_Init(void);
extern void Scan_Key(void);
extern void Key_scan(void);

#endif
