#include "key.h"

// 定义按键引脚
#define KEY1_PIN P50
#define KEY2_PIN P51
#define KEY3_PIN P16
#define KEY4_PIN P17

uint8 keystroke_label = 0; // 按下的是哪个键
uint8 key_last_status[4] = {0};
uint8 key_status[4] = {0};
uint8 key_flag[4] = {0};

void Keystroke_Scan(void)
{
    uint8 i = 0;
    keystroke_label = 0;

    // 保存按键状态
    for (i = 0; i < 4; i++)
    {
        key_last_status[i] = key_status[i];
    }
    key_status[0] = KEY1_PIN; // 按键按下则值为1
    key_status[1] = KEY2_PIN;
    key_status[2] = KEY3_PIN;
    key_status[3] = KEY4_PIN;

    for (i = 0; i < 4; i++)
    {
        if (key_status[i] && !key_last_status[i])
        {
            keystroke_label = i + 1;
            BEEP = 1;
            delay_ms(5);
            BEEP = 0;
            break;  // 一次只响应一个按键，所以有一个按键响应则可以跳出循环
        }
    }
}
