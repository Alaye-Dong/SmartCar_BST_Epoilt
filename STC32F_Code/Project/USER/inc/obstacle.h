#ifndef __OBSTACLE_H_
#define __OBSTACLE_H_

#include "headfile.h"

typedef enum OBSTACLE_STATE
{
    OBSTACLE_NONE = 0,
    OBSTACLE_PRE,
    OBSTACLE_TURN_RIGHT,    // ����ʻ�����ߣ�����
    OBSTACLE_TURN_LEFT_BACK // ���ú�����ʻ������
} OBSTACLE_STATE_enmu;

void ToF_Init(void);
void Obstacle_Recognition(void);
void Obstacle_Turn_Process(void);

#endif