#include "headfile.h"

#define BEEP P32

void BEEP_Init(void)
{
    gpio_mode(P3_2, GPO_PP); // ������
}

void BEEP_ON_ms(int16 ms)
{
    BEEP = 1;
    delay_ms(ms);
    BEEP = 0;
}

void BEEP_ON(void)
{
    BEEP = 1;
}

void BEEP_OFF(void)
{
    BEEP = 0;
}

void BEEP_Toggle(void)
{
    BEEP = !BEEP;
}

/*
 * @name:void Beep_Buzzing(uint8_t time_ms,uint16_t buzz_num);
 * @function: ����
 * @param:  time_ms:����ʱ��   buzz_num:��������
 * @return:none
 */
void Beep_Buzzing(uint8 time_ms, uint16 buzz_num)
{
    BEEP_OFF();
	for(buzz_num = buzz_num * 2; buzz_num > 0; buzz_num--) {
        BEEP_Toggle();
        delay_ms(time_ms);
	}
	BEEP_OFF();
}

