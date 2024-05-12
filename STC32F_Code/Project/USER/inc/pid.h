#ifndef _PID_H_
#define _PID_H_

#include "headfile.h"

typedef struct
{
    float KP;
    float KI;
    float KD;
    float KP_2;
    float KD_2;
} PIDTypeDef;

void PID_Parameter_Init(PIDTypeDef *sptr, float KP, float KI, float KD, float KP_2, float KD_2);
void PIDType_Init(void);
void PID_Init(void);
void PID_Process(void);
void Direction_PID(void);
void Left_Speed_PID(void);
void Right_Speed_PID(void);
int16 clamp(int16 value, int16 min_value, int16 max_value);

#endif