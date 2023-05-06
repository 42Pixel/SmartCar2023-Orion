#include "key.h"

#define KEY1 G0
#define KEY2 G1
#define KEY3 G2
#define KEY4 G3

#define BEEP_PIN   D12       //蜂鸣器引脚


int Beep_flag;

void Beep_Set(){
    static int count=0;
    if(Beep_flag) {
        gpio_set_level(BEEP_PIN, GPIO_HIGH);
        count++;
    }
    else if(!Beep_flag)
        gpio_set_level(BEEP_PIN, GPIO_LOW);
    if(count>=2){
        count=0;
        Beep_flag=0;
    }
}

void Key_Init(void){
    //按键引脚初始化
    gpio_init(G0, GPI, 1, GPI_PULL_UP);
    gpio_init(G1, GPI, 1, GPI_PULL_UP);
    gpio_init(G2, GPI, 1, GPI_PULL_UP);
    gpio_init(G3, GPI, 1, GPI_PULL_UP);
    //拨码开关引脚初始化
    gpio_init(D14, GPI, 1, GPI_PULL_UP);
    gpio_init(D15, GPI, 1, GPI_PULL_UP);
    //蜂鸣器引脚初始化
    gpio_init(BEEP_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);                        // 初始化 BUZZER_PIN 输出 默认低电平 推挽输出模式
}



//按键初始状态为0
//如果状态0或状态1的某按键低电平，计数加一
//如果状态0或状态1的某按键一旦高电平，则回到状态0，计数清零
//如果处于状态0的某按键计数达到FIRST_COUNTER_MAX，则进入状态1，并执行相应动作，计数清零
//如果处于状态1的某按键计数达到SECOND_COUNTER_MAX，则留在状态1，并执行相应动作，计数清零
//该函数要求10ms执行一次
void Key_Scans(void)
{
    static uint8 counter[4], status[4], trigger[4];
    //检查Key1
    if (gpio_get(KEY1)==0)
    {
        counter[Key1]++;
        if (counter[Key1]>=(status[Key1]==0?STATUS0_COUNTER_MAX:STATUS1_COUNTER_MAX))
        {
            status[Key1] = 1;
            trigger[Key1] = 1;
            counter[Key1] = 0;
        }
    }
    else
    {
        status[Key1] = 0;
        counter[Key1] = 0;
    }

    //检查Key2
    if (gpio_get(KEY2)==0)
    {
        counter[Key2]++;
        if (counter[Key2]>=(status[Key2]==0?STATUS0_COUNTER_MAX:STATUS1_COUNTER_MAX))
        {
            status[Key2] = 1;
            trigger[Key2] = 1;
            counter[Key2] = 0;
        }
    }
    else
    {
        status[Key2] = 0;
        counter[Key2] = 0;
    }

    //检查Key3
    if (gpio_get(KEY3)==0)
    {
        counter[Key3]++;
        if (counter[Key3]>=(status[Key3]==0?STATUS0_COUNTER_MAX:STATUS1_COUNTER_MAX))
        {
            status[Key3] = 1;
            trigger[Key3] = 1;
            counter[Key3] = 0;
        }
    }
    else
    {
        status[Key3] = 0;
        counter[Key3] = 0;
    }

    //检查Key4
    if (gpio_get(KEY4)==0)
    {
        counter[Key4]++;
        if (counter[Key4]>=(status[Key4]==0?STATUS0_COUNTER_MAX:STATUS1_COUNTER_MAX))
        {
            status[Key4] = 1;
            trigger[Key4] = 1;
            counter[Key4] = 0;
        }
    }
    else
    {
        status[Key4] = 0;
        counter[Key4] = 0;
    }

    //执行动作
    if (trigger[Key1])
    {
        Key1_Action();
        trigger[Key1] = 0;
    }
    if (trigger[Key2])
    {
        Key2_Action();
        trigger[Key2] = 0;
    }
    if (trigger[Key3])
    {
        Key3_Action();
        trigger[Key3] = 0;
    }
    if (trigger[Key4])
    {
        Key4_Action();
        trigger[Key4] = 0;
    }
}

void Key1_Action(void)
{
    Speed_Target+=200;
    Beep_flag=1;
}

void Key2_Action(void)
{
  Speed_Target=0;
    Beep_flag=1;
}


void Key3_Action(void){

    point++;
   Beep_flag=1;
}

void Key4_Action(void)
{
    Point_write();
    Beep_flag=1;
}