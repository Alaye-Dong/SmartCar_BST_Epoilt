#include "key.h"

// 定义按键引脚
#define KEY1_PIN P50
#define KEY2_PIN P51
#define KEY3_PIN P16
#define KEY4_PIN P17

uint8 keystroke_label = 0; // 按压状态
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
    key_status[0] = KEY1_PIN;
    key_status[1] = KEY2_PIN;
    key_status[2] = KEY3_PIN;
    key_status[3] = KEY4_PIN;

    for (i = 0; i < 4; i++)
    {
        if (key_status[i] && !key_last_status[i])
        {
            key_flag[i] = 1;
        }
        if (key_flag[i])
        {
            key_flag[i] = 0; // 使用按键之后，应该清除标志位
            keystroke_label = i + 1;
            BEEP = 1;
            delay_ms(5);
            BEEP = 0;
        }
    }
}
