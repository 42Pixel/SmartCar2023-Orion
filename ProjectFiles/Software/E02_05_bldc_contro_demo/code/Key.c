#include "Key.h"

#define BUZZER_PIN              (P33_10)

void Key_Init(void){
    gpio_init(BUZZER_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);                        // 蜂鸣器初始化 BUZZER_PIN 输出 默认低电平 推挽输出模式
    key_init(10);
}



