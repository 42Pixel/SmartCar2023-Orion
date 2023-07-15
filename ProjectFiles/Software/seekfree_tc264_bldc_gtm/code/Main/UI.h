#ifndef _UI_H_
#define _UI_H_

#include "zf_common_headfile.h"
#include "stdbool.h"

enum UI_Page{
    InfoPage,
    SetPage,
    GPSPage,
    AboutPage
};

extern enum UI_Page Page_Num;
extern bool Gui_Page_Refersh;
extern void UI(void);

#endif
