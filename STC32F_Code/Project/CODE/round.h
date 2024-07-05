#ifndef __ROUND_H_
#define __ROUND_H_

#include "headfile.h"

typedef enum ROUND_STATE // RIGHT_ANGLE_STATE��һ����ʶ�������Կ���������ϵ����֣���һ����ѡ����ǿ��п��޵��
{
    ROUND_NONE = 0,
    ROUND_PRE,

    ROUND_LEFT_PRE,     // Ԥ�뻷
    ROUND_LEFT_TURN_IN, // �뻷
    ROUND_LEFT_IN,      // ����
    ROUND_LEFT_TURN_OUT,
    ROUND_LEFT_OUT,

    ROUND_RIGHT_PRE,
    ROUND_RIGHT_TURN_IN,
    ROUND_RIGHT_IN,
    ROUND_RIGHT_TURN_OUT,
    ROUND_RIGHT_OUT
} ROUND_STATE_enmu;

extern ROUND_STATE_enmu round_flag;
extern float round_left_proximity_coeff;
extern float round_right_proximity_coeff;

void Round_Recognition(void);
void Round_Turn_Process(void);
void Round_Type_Auto_Deal(void);
void Round_Type_Manual_Deal(void);

#endif