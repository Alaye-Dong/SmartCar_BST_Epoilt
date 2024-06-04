#ifndef _RIGHT_ANGLE_H_
#define _RIGHT_ANGLE_H_

#include "headfile.h"

typedef enum RIGHT_ANGLE_STATE // RIGHT_ANGLE_STATE是一个标识符，可以看成这个集合的名字，是一个可选项，即是可有可无的项。
{
    RIGHT_ANGLE_NONE = 0,
    RIGHT_ANGLE_TURN_LEFT,
    RIGHT_ANGLE_TURN_RIGHT
} RIGHT_ANGLE_STATE_enmu;

extern RIGHT_ANGLE_STATE_enmu right_angle_flag;

void Right_Angle_Recognition(void);
void Right_Angle_Turn_Process(void);

#endif