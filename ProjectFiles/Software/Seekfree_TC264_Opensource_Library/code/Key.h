#ifndef _KEY_H_
#define _KEY_H_

#include "zf_common_headfile.h"
#include "stdbool.h"

enum Set_Action{
    Servo_PD_KP,
    Servo_PD_KD,
    Speed
};

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

extern bool Run_Start_Status;
extern bool Servo_Status;
extern bool Gui_Page_Refersh;
extern bool Gui_Status;
extern uint8 Key_Num;

extern void Key_Init(void);
extern void Key_scan(void);
extern void Key_Active(void);

#endif
