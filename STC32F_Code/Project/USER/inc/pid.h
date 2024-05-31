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

typedef struct
{
    int16 normal;
    float boost_factor;
    float deceleration_factor;
    int16 target;
    int16 target_left;
    int16 target_right;
} SpeedTypeDef;

extern SpeedTypeDef speed;

extern PIDTypeDef direction, motor_left, motor_right;

extern float direction_output;

void PID_Parameter_Init(PIDTypeDef *sptr, float KP, float KI, float KD, float KP_2, float KD_2);
void PIDType_Init(void);
void PID_Init(void);
void PID_Process(void);
void Direction_PID(void);
void Left_Speed_PID(void);
void Right_Speed_PID(void);
void Speed_Contrl(void);

#endif