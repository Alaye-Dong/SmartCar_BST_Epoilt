#ifndef __ROUND_H_
#define __ROUND_H_

#include "headfile.h"

typedef enum ROUND_STATE // RIGHT_ANGLE_STATE是一个标识符，可以看成这个集合的名字，是一个可选项，即是可有可无的项。
{
    ROUND_NONE = 0,
    ROUND_PRE,

    ROUND_LEFT_PRE,     // 预入环
    ROUND_LEFT_TURN_IN, // 入环
    ROUND_LEFT_IN,      // 环内
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