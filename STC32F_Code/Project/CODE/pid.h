#ifndef __PID_H_
#define __PID_H_

#include "headfile.h"

typedef struct PID
{
    float KP;
    float KI;
    float KD;
    float KP_2;
    float KD_2;
    float KF; // Ç°À¡ÏµÊý
} PIDTypeDef;

typedef struct Speed
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

extern uint16 position_loss_timer;

void PID_Parameter_Init(PIDTypeDef *sptr, float KP, float KI, float KD, float KP_2, float KD_2, float KF);
void PIDType_Init(void);
void PID_Init(void);
void PID_Process(void);

void Direction_PID(void);
void Left_Speed_PID(void);
void Right_Speed_PID(void);
void Speed_Contrl(void);
float Increment_PI_Dynamic_P_MAX(int16 target_speed_encoder, int16 encoder_now);

uint8 Fuzzify(float value);
uint8 Fuzzy_Rule(int error, int delta_error);

void Fuzzy_PID_Control(float error, float delta_error, PIDTypeDef *sptr);

#endif