#ifndef CODE_MOTOR_H_
#define CODE_MOTOR_H_

#include "zf_common_headfile.h"

extern int8 Speed_Target;

extern int16 Encoder_Get(void);
extern void Motor_Init(void);
extern void Motor_Control(void);


#endif /* CODE_MOTOR_H_ */
