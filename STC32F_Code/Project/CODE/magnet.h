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

typedef struct
{
    int16 V_sum;
    int16 H_sum;
    int16 S_sum;
    int16 V_difference;
    int16 H_difference;
    int16 S_difference;
} InductorMathTypeDef;

extern InductorMathTypeDef inductor_math;

extern int16 ADC_values[INDUCTORS][SAMPLES];
extern int16 ADC_filtered_value[];

extern int16 inductor[];

extern int16 position_vector_modulus, position_difference_weighted, position_normal, position; // position大于0表示车偏右应左转，小于0表示车偏左应右转

extern uint8 position_loss_stop_protect_flag;

void Magnet_ADC_Init(void);
void Magnet_ADC_Read(void);
void Inductor_Process(void);
void Magnet_ADC_Filter(void);
// void Bubble_Sort_ADC(void);
void Inductor_Normal(void);
void Inductor_Math_Pre_Calculate(InductorMathTypeDef *sptr);

void Position_Analyse(void);
void Position_Loss_Remember_Protect(uint8 protect_mode);

#endif