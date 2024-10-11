#include "key.h"

// ���尴������
#define KEY1_PIN P50
#define KEY2_PIN P51
#define KEY3_PIN P16
#define KEY4_PIN P17

#define KEYS 4

uint8 keystroke_label = 0; // ���µ����ĸ��� 1 2 3 4

void Keystroke_Scan(void)
{
    static uint8 key_last_status[KEYS] = {1, 1, 1, 1}; // ��ʼ����״̬����֤���μ��ʱkeystroke_label��Ϊ0
    static uint8 key_status[KEYS] = {0};
    uint8 i = 0;

    keystroke_label = 0; // Ĭ��û�а�������

    key_status[0] = KEY1_PIN; // ��ȡ����������������ֵΪ1
    key_status[1] = KEY2_PIN;
    key_status[2] = KEY3_PIN;
    key_status[3] = KEY4_PIN;

    for (i = 0; i < KEYS; i++)
    {
        if (key_status[i] && !key_last_status[i])
        {
            keystroke_label = i + 1; // +1ʹֵ���밴�����
            // BEEP_ON_ms(5);
        }
        key_last_status[i] = key_status[i]; // ���水��״̬
    }
}
