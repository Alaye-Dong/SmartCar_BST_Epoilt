#ifndef _MAGNET_H_
#define _MAGNET_H_

#include "headfile.h"

#define INDUCTORS 6 // ��еĸ���
#define SAMPLES 5   // ���βɼ�������

#define EXTREME_NUMBER 1 // �����������Сֵ�ĸ���

// ˮƽ 32��H������ֱ 05��V����б�� 41��S��
#define LEFT_V 0
#define RIGHT_V 5
#define LEFT_H 3
#define RIGHT_H 2
#define LEFT_S 4
#define RIGHT_S 1

extern int16 ADC_value[INDUCTORS][SAMPLES];
extern int16 ADC_filtered_value[];

extern int16 inductor_left_V, inductor_right_V, inductor_left_H, inductor_right_H, inductor_left_S, inductor_right_S;
extern int16 position_left, position_right, position; //position����0��ʾ��ƫ��Ӧ��ת��С��0��ʾ��ƫ��Ӧ��ת

void Magnet_ADC_Init(void);
void Magnet_ADC_Read(void);
void Inductor_Process(void);
void Magnet_ADC_Filter(void);
void Bubble_Sort_ADC(void);
void Inductor_Normal(void);
void Inductor_Analyse(void);
void Magnet_ADC_Print(void);

#endif