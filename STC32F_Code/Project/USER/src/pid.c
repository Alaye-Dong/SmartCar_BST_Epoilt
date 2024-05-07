#include "pid.h"

PIDTypeDef direction;

void PID_Parameter_Init(PIDTypeDef *sptr, float KP, float KI, float KD, float KP_2, float KD_2)
{
    sptr->KP = KP;
    sptr->KI = KI;
    sptr->KD = KD;
    sptr->KP_2 = KP_2;
    sptr->KD_2 = KD_2;
}

void PIDType_Init(void)
{
    PID_Parameter_Init(&direction, 0.0, 0.0, 0.0, 0.0, 0.0);
}

float direction_output;
int16 position_last;
// PID转向控制
void Direction_PID(void)
{
    direction_output = position * direction.KP + (position - position_last) * direction.KD; //+ abs(position) * position * direction.KP_2 + groy_z * KD2; //加入二次项，转向更迅速，直道灵敏度降低。融合陀螺仪，转向增加阻尼，更平稳。
    position_last = position;
}


