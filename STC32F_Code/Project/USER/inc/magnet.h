#ifndef _MAGNET_H_
#define _MAGNET_H_

#include "headfile.h"

void Magnet_ADC_Init(void);
void Magnet_ADC_Read(void);
void Magnet_Process(void);
void Magnet_ADC_Filter(void);
void Bubble_Sort_ADC(void);

#endif