#include "key.h"



//���뿪��״̬��?? ON??0
uint8 sw1_status;
uint8 sw2_status;

uint8 key_last_status[5]={0};
uint8 key_status[5]={0};
uint8 key_flag[5]={0};

void Keystroke_Scan(void)
{

    uint8 i = 0;
    keystrokeLabel = 0;
    
    //��ȡ���뿪��״??
    sw1_status = SW1_PIN;
    sw2_status = SW2_PIN;
    
    // ���水��״??
    for ( i = 0; i < 4; i++) {
        key_last_status[i] = key_status[i];
    }
    key_status[0] = KEY1_PIN;
    key_status[1] = KEY2_PIN;
    key_status[2] = KEY3_PIN;
    key_status[3] = KEY4_PIN;

    for ( i = 0; i < 4; i++) {
        if (key_status[i] && !key_last_status[i]) {
            key_flag[i] = 1;
        }
        if (key_flag[i]) {
            key_flag[i] = 0; // ʹ�ð���֮��Ӧ�������־λ
            keystrokeLabel = i + 1;
            BEEP = 1;
            delay_ms(5);
            BEEP = 0;
        }
    }
    
}
