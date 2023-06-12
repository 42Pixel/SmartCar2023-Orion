

#ifndef _PRINTF_H_
#define _PRINTF_H_
#include "zf_common_headfile.h"

//void    printf(const char *format, ...);
void myprintf(const int8 *format, ...);
uint32 zf_sprintf(int8 *buff, const int8 *format, ...);

#endif
