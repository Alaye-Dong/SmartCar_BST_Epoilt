#ifndef __BEEP_H_
#define __BEEP_H_

#include "headfile.h"

void BEEP_Init(void);
void BEEP_ON_ms(int16 ms);
void BEEP_ON(void);
void BEEP_OFF(void);
void BEEP_Toggle(void);
void Beep_Buzzing(uint8 time_ms, uint8 buzz_num);

#endif