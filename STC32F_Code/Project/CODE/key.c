#include "key.h"

// 定义按键引脚
#define KEY1_PIN P50
#define KEY2_PIN P51
#define KEY3_PIN P16
#define KEY4_PIN P17

#define KEYS 4

uint8 keystroke_label = 0; // 按下的是哪个键 1 2 3 4

void Keystroke_Scan(void)
{
    static uint8 key_last_status[KEYS] = {1, 1, 1, 1}; // 初始化键状态，保证初次检测时keystroke_label能为0
    static uint8 key_status[KEYS] = {0};
    uint8 i = 0;

    keystroke_label = 0; // 默认没有按键按下

    key_status[0] = KEY1_PIN; // 读取按键，按键按下则值为1
    key_status[1] = KEY2_PIN;
    key_status[2] = KEY3_PIN;
    key_status[3] = KEY4_PIN;

    for (i = 0; i < KEYS; i++)
    {
        if (key_status[i] && !key_last_status[i])
        {
            keystroke_label = i + 1; // +1使值对齐按键编号
            // BEEP_ON_ms(5);
        }
        key_last_status[i] = key_status[i]; // 保存按键状态
    }
}
