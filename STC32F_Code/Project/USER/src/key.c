#include "key.h"

// ���尴������
#define KEY1_PIN P50
#define KEY2_PIN P51
#define KEY3_PIN P16
#define KEY4_PIN P17

uint8 keystroke_label = 0; // ���µ����ĸ���
uint8 key_last_status[4] = {0};
uint8 key_status[4] = {0};
uint8 key_flag[4] = {0};

void Keystroke_Scan(void)
{
    uint8 i = 0;
    keystroke_label = 0;

    // ���水��״̬
    for (i = 0; i < 4; i++)
    {
        key_last_status[i] = key_status[i];
    }
    key_status[0] = KEY1_PIN; // ����������ֵΪ1
    key_status[1] = KEY2_PIN;
    key_status[2] = KEY3_PIN;
    key_status[3] = KEY4_PIN;

    for (i = 0; i < 4; i++)
    {
        if (key_status[i] && !key_last_status[i])
        {
            keystroke_label = i + 1;
            BEEP_ON_ms(5);
            break;  // һ��ֻ��Ӧһ��������������һ��������Ӧ���������ѭ��
        }
    }
}
