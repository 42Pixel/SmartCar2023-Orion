#ifndef _KEY_H_
#define _KEY_H_

#include "zf_common_headfile.h"

enum Keycode{
    Key1,
    Key2,
    Key3,
    Key4
};

extern void Key_Init(void);
extern void Scan_Key(void);

#endif
