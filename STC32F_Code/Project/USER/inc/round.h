#ifndef _ROUND_H_
#define _ROUND_H_

#include "headfile.h"

typedef enum ROUND_STATE //RIGHT_ANGLE_STATE是一个标识符，可以看成这个集合的名字，是一个可选项，即是可有可无的项。
{
    ROUND_NONE = 0, 
    ROUND_LEFT_IN,
    ROUND_LEFT_OUT,     
    ROUND_RIGHT_IN,
    ROUND_RIGHT_OUT
} ROUND_STATE_enmu;

extern ROUND_STATE_enmu round_flag;
extern float round_right_knn;

void Round_Recognition(void);
void Round_Turn_Process(void);

#endif