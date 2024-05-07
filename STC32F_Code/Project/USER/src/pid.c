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
// PIDת�����
void Direction_PID(void)
{
    direction_output = position * direction.KP + (position - position_last) * direction.KD; //+ abs(position) * position * direction.KP_2 + groy_z * KD2; //��������ת���Ѹ�٣�ֱ�������Ƚ��͡��ں������ǣ�ת���������ᣬ��ƽ�ȡ�
    position_last = position;
}


