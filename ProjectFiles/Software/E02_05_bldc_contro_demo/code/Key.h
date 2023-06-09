#include "zf_common_headfile.h"

#ifndef KEY_H_
#define KEY_H_

enum Keycode{
    Key1,
    Key2,
    Key3,
    Key4
};

extern void Key_Init(void);
extern void Scan_Key(void);

#endif
