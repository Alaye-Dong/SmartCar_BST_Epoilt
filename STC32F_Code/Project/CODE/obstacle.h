#ifndef __OBSTACLE_H_
#define __OBSTACLE_H_

#include "headfile.h"

typedef enum OBSTACLE_STATE
{
    OBSTACLE_NONE = 0,
    OBSTACLE_PRE,
    OBSTACLE_TURN_RIGHT,    // 向右驶出中线，避让
    OBSTACLE_STRAIGHT,      // 直行
    OBSTACLE_TURN_LEFT_BACK // 避让后向左驶回中线
} OBSTACLE_STATE_enmu;

extern OBSTACLE_STATE_enmu obstacle_flag;

void ToF_Init(void);
void Obstacle_Recognition(void);
void Obstacle_Turn_Process(void);

#endif