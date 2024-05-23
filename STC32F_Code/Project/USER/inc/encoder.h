#ifndef __ENCODER_H
#define __ENCODER_H

#include "headfile.h"

#define ENCODER_DIR_1 P35 // 左轮编码器方向脚，电机轴针对自己，值为1代表逆时针转(向前正转)
#define ENCODER_DIR_2 P53 // 右轮编码器方向脚

typedef struct
{
    int16 encoder_now;
    int16 encoder_last;
} EncoderTypeDef;

extern EncoderTypeDef encoder_left, encoder_right;

void Encoder_Init(void);

void Encoder_Parameter_Init(EncoderTypeDef *sptr);

void EncoderType_Init(void);

void Read_Encoder(void);

void Distance_Calculation(void);

#endif