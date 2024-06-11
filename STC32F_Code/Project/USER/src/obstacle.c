#include "obstacle.h"

OBSTACLE_STATE_enmu obstacle_flag = OBSTACLE_NONE;
//! dl1b_distance_mm TOF���صľ��룬����ɿ�SEEKFREE_DL1B.c����

// TODO ������ֵ��ʵ��ó�

void ToF_Init(void)
{
    if (dl1b_init())
    {
        lcd_clear(RED);
        lcd_showstr(1 * 8, 1, "TOF_ERROR");
    }
}

void Obstacle_Recognition(void)
{
    dl1b_get_distance();

    if (dl1b_distance_mm <= 850 && dl1b_distance_mm >= 300 && inductor[LEFT_H] >= 30 && inductor[RIGHT_H] >= 40) // �ϰ�ʶ�� �����ںϽ��ٶ��жϣ��ڱȽ�ֱ�������ű��ϣ�
    {
        obstacle_flag = OBSTACLE_PRE;
    }
}

void Obstacle_Turn_Process(void)
{
    element_busy_flag = 1;
    switch (obstacle_flag)
    {
    case OBSTACLE_PRE:
        BEEP_ON();
        obstacle_flag = OBSTACLE_TURN_RIGHT;
        break;

    case OBSTACLE_TURN_RIGHT:
        position = -50;
        IMU_Yaw_Angle_Get_Control(ON);
        Distance_Calculation();
        if (real_distance > 50 && FUNC_ABS(yaw_angle) > 50)
        {
            IMU_Yaw_Angle_Get_Control(RESET);
            Distance_Reset();
            obstacle_flag = OBSTACLE_TURN_LEFT_BACK;
        }
        break;

    case OBSTACLE_TURN_LEFT_BACK:
        position = 50;
        IMU_Yaw_Angle_Get_Control(ON);
        Distance_Calculation();
        if (real_distance > 50 && FUNC_ABS(yaw_angle) > 50)
        {
            IMU_Yaw_Angle_Get_Control(RESET);
            Distance_Reset();
            obstacle_flag = OBSTACLE_NONE;
            element_busy_flag = 0;
        }
        break;
    }
}