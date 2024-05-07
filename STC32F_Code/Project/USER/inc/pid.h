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

#endif