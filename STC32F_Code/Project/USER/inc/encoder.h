#ifndef __ENCODER_H
#define __ENCODER_H

#include "headfile.h"

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