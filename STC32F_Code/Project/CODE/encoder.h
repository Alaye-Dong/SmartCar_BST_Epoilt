#ifndef __ENCODER_H_
#define __ENCODER_H_

#include "headfile.h"

#define ENCODER_DIR_1 P35 // ���ֱ���������� ������ŵ���İ�װ����棬��ʱ�뷽����תΪ��ת�ķ���
#define ENCODER_DIR_2 P53 // ���ֱ����������

typedef struct
{
    int16 encoder_now;
    int16 encoder_last;
    int16 encoder_filtered;
} EncoderTypeDef;

extern EncoderTypeDef encoder_left, encoder_right;
extern float car_distance_cm;

void Encoder_Init(void);

void Encoder_Parameter_Init(EncoderTypeDef *sptr);

void EncoderType_Init(void);

void Encoder_Process(void);

void Read_Encoder(void);

void Encoder_Filter(void);

void Distance_Calculation(void);

void Distance_Reset(void);

#endif