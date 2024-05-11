#ifndef _MAGNET_H_
#define _MAGNET_H_

#include "headfile.h"


extern int16 ADC_filtered_value[];

extern int16 inductor_left_V, inductor_right_V, inductor_left_H, inductor_right_H, inductor_left_S, inductor_right_S;
extern int16 position_left, position_right, position; //position����0��ʾ��ƫ��Ӧ��ת��С��0��ʾ��ƫ��Ӧ��ת

void Magnet_ADC_Init(void);
void Magnet_ADC_Read(void);
void Inductor_Process(void);
void Magnet_ADC_Filter(void);
void Bubble_Sort_ADC(void);
void Bubble_Sort_Int(int data_array[], int length);
void Inductor_Normal(void);
void Inductor_Analyse(void);

#endif