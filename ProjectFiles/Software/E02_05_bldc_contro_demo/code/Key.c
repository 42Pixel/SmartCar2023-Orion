#include "Key.h"

#define KEY1_GPIO               (P20_6)
#define KEY2_GPIO               (P20_7)
#define KEY3_GPIO               (P11_2)
#define KEY4_GPIO               (P11_3)
#define BUZZER_PIN              (P33_10)

//----------------------------------------------------------------------------------------------------------------
// 函数简介     按键&蜂鸣器初始化
// 参数说明
// 返回参数
// 使用示例     Key_Init();
// 备注信息
//----------------------------------------------------------------------------------------------------------------
void Key_Init(void){
    gpio_init(KEY1_GPIO, GPI, 0, GPI_PULL_UP);
    gpio_init(KEY2_GPIO, GPI, 0, GPI_PULL_UP);
    gpio_init(KEY3_GPIO, GPI, 0, GPI_PULL_UP);
    gpio_init(KEY4_GPIO, GPI, 0, GPI_PULL_UP);
    gpio_init(BUZZER_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);            // 初始化 BUZZER_PIN 输出 默认低电平 推挽输出模式
    gpio_set_level(BUZZER_PIN, GPIO_LOW);
}

//----------------------------------------------------------------------------------------------------------------
// 函数简介     蜂鸣器响
// 参数说明
// 返回参数
// 使用示例     Beep();
// 备注信息
//----------------------------------------------------------------------------------------------------------------
void Beep(void){
    gpio_set_level(BUZZER_PIN, GPIO_HIGH);
    system_delay_ms(10);
    gpio_set_level(BUZZER_PIN, GPIO_LOW);
}

//----------------------------------------------------------------------------------------------------------------
// 函数简介     按键扫秒
// 参数说明
// 返回参数
// 使用示例     Check_Key();
// 备注信息     放主函数;
//---------------------------------------------------------------------------------------------------------------
void Scan_Key(void){
    static uint8 K[4];

    if (gpio_get_level(KEY1_GPIO)==0){
        system_delay_ms(15);
        if (gpio_get_level(KEY1_GPIO)==0)                   //消抖
            K[Key1] = TRUE;
    }

    if (gpio_get_level(KEY2_GPIO)==0){
        system_delay_ms(15);
        if (gpio_get_level(KEY2_GPIO)==0)                   //消抖
            K[Key2] = TRUE;
    }

    if (gpio_get_level(KEY3_GPIO)==0){
        system_delay_ms(15);
        if (gpio_get_level(KEY3_GPIO)==0)                   //消抖
            K[Key3] = TRUE;
    }

    if (gpio_get_level(KEY4_GPIO)==0){
        system_delay_ms(15);
        if (gpio_get_level(KEY4_GPIO)==0)                   //消抖
            K[Key4] = TRUE;
    }


    if (K[Key1]){
        Beep();
        s_pid_KP+=0.1;
        K[Key1] = FALSE;
    }
    if (K[Key2]){
        Beep();
        s_pid_KP-=0.1;
        K[Key2] = FALSE;
    }
    if (K[Key3]){
        Beep();
        s_pid_KD+=0.1;
        K[Key3] = FALSE;
    }
    if (K[Key4]){
        Beep();
        s_pid_KD-=0.1;
        K[Key4] = FALSE;
    }
}
