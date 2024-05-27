#ifndef __ENCODER_H
#define __ENCODER_H

#include "headfile.h"

#define ENCODER_DIR_1 P35 // 左轮编码器方向脚 正面对着电机的安装轴端面，逆时针方向旋转为正转的方向
#define ENCODER_DIR_2 P53 // 右轮编码器方向脚

typedef struct
{
    int16 encoder_now;
    int16 encoder_last;
    int16 encoder_filtered;
} EncoderTypeDef;

extern EncoderTypeDef encoder_left, encoder_right;
extern int32 car_distance;
extern float real_distance;

void Encoder_Init(void);

void Encoder_Parameter_Init(EncoderTypeDef *sptr);

void EncoderType_Init(void);

void Encoder_Process(void);

void Read_Encoder(void);

void Encoder_Filter(void);

void Distance_Calculation(void);

void Distance_Reset(void);

#endif