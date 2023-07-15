#include "Key.h"

#define KEY1_GPIO               (P20_6)
#define KEY2_GPIO               (P20_7)
#define KEY3_GPIO               (P11_2)
#define KEY4_GPIO               (P11_3)
#define SWITCH1                 (P33_12)
#define SWITCH2                 (P33_13)
#define BUZZER_PIN              (P33_10)

uint8 Key_Num=0;

bool Run_Start_Status=false;
bool Run_Status=false;
bool Servo_Status=false;
bool Gui_Page_Refersh=true;
bool Gui_Status=true;


Key_status Status={1,1,1,1};        // 当前按键状态
Last_status L_status;               // 上一次按键状态
Key_flag Flag;                      // 按键触发标志位
enum Set_Action set_action;

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
    gpio_init(SWITCH1, GPI, GPIO_HIGH, GPI_FLOATING_IN);    // 初始化 SWITCH1 输入 默认高电平 浮空输入
    gpio_init(SWITCH2, GPI, GPIO_HIGH, GPI_FLOATING_IN);    // 初始化 SWITCH2 输入 默认高电平 浮空输入

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


void Key_Active(void){
    switch (Key_Num){
        case 1:
            Beep();
                if(Page_Num==InfoPage){          //第一页参数设置
                    Run_Start_Status=true;       //开跑
                    Gui_Status=false;            //关闭UI显示
                }

                if(Page_Num==SetPage){          //第二页参数设置
                    switch (set_action) {
                        case Servo_PD_KP : Servo_pid_KP+=0.5;break;
                        case Servo_PD_KD : Servo_pid_KD+=0.01;break;
                        case Speed : Start_Speed+=5;break;
                        default:break;
                    }
                }
                if(Page_Num==GPSPage){
                    Point_Get();
                }



                Flag.key1 = 0;                  // 使用按键之后，应该清除标志位
                break;
        case 2:
            Beep();
                if(Page_Num==InfoPage){        //第一页参数设置
                    Speed_Duty=0;              //速度置零
                    pwm_set_duty(SERVO_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY(100));            //舵机回正
                    Servo_Status=false;        //锁定舵机控制
                    Run_Status=false;
                    Gui_Status=true;           //开启UI显示
                }

                if(Page_Num==SetPage){          //第二页参数设置
                    if(Page_Num==SetPage){          //第二页参数设置
                        switch (set_action) {
                            case Servo_PD_KP : Servo_pid_KP-=0.5;break;
                            case Servo_PD_KD : Servo_pid_KD-=0.01;break;
                            case Speed : Start_Speed-=5;break;
                            default:break;
                        }
                    }

                    if(Page_Num==GPSPage){
                        Point_Count++;
                    }

                }



                Flag.key2 = 0;                 // 使用按键之后，应该清除标志位
                break;
        case 3:
            Beep();
                if(Page_Num==InfoPage){        //第一页参数设置
                    Point_Count=0;             //点位置零
                    Q_info.q0=1.0f;
                    Q_info.q1=0.0f;
                    Q_info.q2=0.0f;
                    Q_info.q3=0.0f;
                    eulerAngle.yaw = 0.0f;
                    Run_Status=false;
                }

                if(Page_Num==SetPage){          //第二页参数设置
                    set_action++;               //切换设置项
                    if(set_action>2)
                        set_action=0;
                }

                if(Page_Num==GPSPage){
                    Point_Count--;
                }

                Flag.key3 = 0;                 // 使用按键之后，应该清除标志位
                break;
        case 4:
            Beep();
                 Page_Num++;                   //  切换页面
                 Gui_Page_Refersh=true;
                 if(Page_Num>2)
                     Page_Num=0;

                 Flag.key4 = 0;                 // 使用按键之后，应该清除标志位
                 break;

    }
    Key_Num=0;
}


//----------------------------------------------------------------------------------------------------------------
// 函数简介     按键扫秒
// 参数说明
// 返回参数
// 使用示例     Key_scan();
// 备注信息     可以放中断;
//--------------------------------------------------------------------------------------------------------------
void Key_scan(void)
{


    L_status.key1 = Status.key1;                              // 保存按键状态
    Status.key1 = gpio_get_level(KEY1_GPIO);                  // 读取当前按键状态
    if(Status.key1 && !L_status.key1)                         // 检测松开按键的动作后   置位标志位
    {
        Key_Num=1;                                            // 标志位 置位 之后，可以使用标志位执行自己想要做的事件
    }

    L_status.key2 = Status.key2;                              // 保存按键状态
    Status.key2 = gpio_get_level(KEY2_GPIO);                  // 读取当前按键状态
    if(Status.key2 && !L_status.key2)                         // 检测松开按键的动作后   置位标志位
    {
        Key_Num=2;                                            // 标志位 置位 之后，可以使用标志位执行自己想要做的事件
    }

    L_status.key3 = Status.key3;                              // 保存按键状态
    Status.key3 = gpio_get_level(KEY3_GPIO);                  // 读取当前按键状态
    if(Status.key3 && !L_status.key3)                         // 检测松开按键的动作后   置位标志位
    {
        Key_Num=3;                                            // 标志位 置位 之后，可以使用标志位执行自己想要做的事件
    }

    L_status.key4 = Status.key4;                             // 保存按键状态
    Status.key4 = gpio_get_level(KEY4_GPIO);                 // 读取当前按键状态
    if(Status.key4 && !L_status.key4)                        // 检测松开按键的动作后   置位标志位
    {
        Key_Num=4;                                           // 标志位 置位 之后，可以使用标志位执行自己想要做的事件
    }
}


