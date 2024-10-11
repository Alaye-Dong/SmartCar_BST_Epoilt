#ifndef __CROSS_H_
#define __CROSS_H_

#include "headfile.h"

typedef enum CROSS_STATE
{
    CROSS_NONE = 0,
    CROSS_IN
} CROSS_STATE_enmu;

extern CROSS_STATE_enmu cross_flag;

void Cross_Recognition(void);
void Cross_Turn_Process(void);

#endif