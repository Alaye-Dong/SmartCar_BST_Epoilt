#ifndef __MAGNET_H_
#define __MAGNET_H_

#include "headfile.h"

#define INDUCTORS 6 // ��еĸ���
#define SAMPLES 5   // ���βɼ�������

#define EXTREME_NUMBER 1 // �����������Сֵ�ĸ���

// ˮƽ��H������ֱ��V����б��S��
enum Inductor_Type
{
    LEFT_V = 0,
    RIGHT_V = 1,
    LEFT_H = 2,
    RIGHT_H = 3,
    LEFT_S = 4,
    RIGHT_S = 5
};
// #define LEFT_V 0
// #define RIGHT_V 1
// #define LEFT_H 2
// #define RIGHT_H 3
// #define LEFT_S 4
// #define RIGHT_S 5

extern int16 ADC_value[INDUCTORS][SAMPLES];
extern int16 ADC_filtered_value[];

extern int16 inductor[];
extern int16 position_left, position_right, position; // position����0��ʾ��ƫ��Ӧ��ת��С��0��ʾ��ƫ��Ӧ��ת

void Magnet_ADC_Init(void);
void Magnet_ADC_Read(void);
void Inductor_Process(void);
void Magnet_ADC_Filter(void);
void Bubble_Sort_ADC(void);
void Inductor_Normal(void);
void Position_Analyse(void);
void Magnet_ADC_Print(void);

#endif