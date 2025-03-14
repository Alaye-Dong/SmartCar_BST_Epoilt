#ifndef __OBSTACLE_H_
#define __OBSTACLE_H_

#include "headfile.h"

typedef enum OBSTACLE_STATE
{
    OBSTACLE_NONE = 0,
    OBSTACLE_PRE,
    OBSTACLE_TURN_RIGHT, // 向右驶出赛道
    OBSTACLE_TURN_LEFT_PRE,
    OBSTACLE_STRAIGHT,       // 直行
    OBSTACLE_TURN_LEFT_BACK, // 避让后向左驶回赛道
    OBSTACLE_DEBUG_STOP      // 调试时停车
} OBSTACLE_STATE_enmu;

extern OBSTACLE_STATE_enmu obstacle_flag;

extern int16 obstacle_on;

void ToF_Init(void);
void Obstacle_Recognition(void);
void Obstacle_Turn_Process(void);

#endif