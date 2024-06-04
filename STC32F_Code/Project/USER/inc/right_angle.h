#ifndef _RIGHT_ANGLE_H_
#define _RIGHT_ANGLE_H_

#include "headfile.h"

typedef enum RIGHT_ANGLE_STATE // RIGHT_ANGLE_STATE��һ����ʶ�������Կ���������ϵ����֣���һ����ѡ����ǿ��п��޵��
{
    RIGHT_ANGLE_NONE = 0,
    RIGHT_ANGLE_TURN_LEFT,
    RIGHT_ANGLE_TURN_RIGHT
} RIGHT_ANGLE_STATE_enmu;

extern RIGHT_ANGLE_STATE_enmu right_angle_flag;

void Right_Angle_Recognition(void);
void Right_Angle_Turn_Process(void);

#endif