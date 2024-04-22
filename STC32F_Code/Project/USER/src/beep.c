#include "headfile.h"

#define BEEP P32

void BEEP_Init(void)
{
    gpio_mode(P3_2, GPO_PP); // ������
}

void BEEP_ON_ms(int ms)
{
    BEEP = 1;
    delay_ms(ms);
    BEEP = 0;
}
