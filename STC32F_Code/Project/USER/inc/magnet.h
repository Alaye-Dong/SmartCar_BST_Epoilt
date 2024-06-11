#ifndef __MAGNET_H_
#define __MAGNET_H_

#include "headfile.h"

#define INDUCTORS 6 // 电感的个数
#define SAMPLES 5   // 单次采集样本数

#define EXTREME_NUMBER 1 // 舍弃的最大最小值的个数

// 水平（H）、垂直（V）和斜向（S）
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
extern int16 position_left, position_right, position; // position大于0表示车偏右应左转，小于0表示车偏左应右转

void Magnet_ADC_Init(void);
void Magnet_ADC_Read(void);
void Inductor_Process(void);
void Magnet_ADC_Filter(void);
void Bubble_Sort_ADC(void);
void Inductor_Normal(void);
void Position_Analyse(void);
void Magnet_ADC_Print(void);

#endif