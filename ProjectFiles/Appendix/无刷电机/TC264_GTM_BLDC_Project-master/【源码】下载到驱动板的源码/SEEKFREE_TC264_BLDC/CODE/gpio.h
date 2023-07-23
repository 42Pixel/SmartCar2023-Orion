#ifndef _GPIO_H
#define _GPIO_H

#include "common.h"




#define LED_ERR_PIN         P21_2
#define LED_RUN_PIN         P21_3
#define LED_MODEL_PIN       P21_4
#define EN_PIN              P11_6

void led_init(void);
void led_output(void);

#endif
