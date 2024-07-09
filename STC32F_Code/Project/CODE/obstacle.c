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

    if (dl1b_distance_mm <= 850 && dl1b_distance_mm >= 300 && inductor_math.V_sum < 30) // �ϰ�ʶ�� �����ںϽ��ٶ��жϣ��ڱȽ�ֱ�������ű��ϣ�
    {
        element_busy_flag = ELEMENT_OBSTACLE;
        obstacle_flag = OBSTACLE_PRE;
    }
}

void Obstacle_Turn_Process(void)
{
    switch (obstacle_flag)
    {
    case OBSTACLE_PRE:
        BEEP_ON();
        obstacle_flag = OBSTACLE_TURN_RIGHT;
        break;

    case OBSTACLE_TURN_RIGHT:
        position = -150;
        IMU_Yaw_Angle_Get_Control(ON);
        Distance_Calculation();
        if (car_distance_cm > 50 && FUNC_ABS(yaw_angle) > 50)
        {
            IMU_Yaw_Angle_Get_Control(RESET);
            Distance_Reset();
            obstacle_flag = OBSTACLE_STRAIGHT;
        }
        break;

    case OBSTACLE_STRAIGHT:
        position = 0;
        Distance_Calculation();
        if (car_distance_cm > 30)
        {
            Distance_Reset();
            obstacle_flag = OBSTACLE_TURN_LEFT_BACK;
        }
        break;

    case OBSTACLE_TURN_LEFT_BACK:
        position = 150;
        IMU_Yaw_Angle_Get_Control(ON);
        Distance_Calculation();
        if (car_distance_cm > 50 && FUNC_ABS(yaw_angle) > 50)
        {
            IMU_Yaw_Angle_Get_Control(RESET);
            Distance_Reset();
            BEEP_OFF();
            obstacle_flag = OBSTACLE_NONE;
        }
        break;

    case OBSTACLE_NONE:
        element_busy_flag = ELEMENT_NONE;
        break;

    default:
        break;
    }
}