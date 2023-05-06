#ifndef _KEY_H
#define _KEY_H

#include "common.h"

//定义驱动板按键
#define KEY1   P21_5

extern uint8 model_state;

void key_init(void);
void key_scan(void);

#endif
