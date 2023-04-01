//
// Created by FLY on 2022/5/2.
//

#ifndef MM32_KEY_H
#define MM32_KEY_H

#include "zf_common_headfile.h"

#define STATUS0_COUNTER_MAX 5
#define STATUS1_COUNTER_MAX 50

enum Keycode
{
    Key1,
    Key2,
    Key3,
    Key4
};


void Key_Init(void);
void Key_Scans(void);

void Beep_Set();

void Key1_Action(void);
void Key2_Action(void);
void Key3_Action(void);
void Key4_Action(void);

extern int Beep_flag;

#endif
