#include "obstacle.h"

OBSTACLE_STATE_enmu obstacle_flag = OBSTACLE_NONE;
int16 obstacle_on = 1;
//! dl1b_distance_mm TOF返回的距离，由逐飞库SEEKFREE_DL1B.c定义

// TODO 具体阈值待实验得出

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
    static obstacle_number = 0;
    dl1b_get_distance();

    if (obstacle_number < 1 && dl1b_distance_mm <= 850 && dl1b_distance_mm >= 300 && inductor_math.V_sum < 30) // 障碍识别 可以融合角速度判断（在比较直的赛道才避障）
    {
        element_busy_flag = ELEMENT_OBSTACLE;
        obstacle_flag = OBSTACLE_PRE;
        obstacle_number++;
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
        position = -76;
        IMU_Yaw_Angle_Get_Control(ON);
        Distance_Calculation();
        if (FUNC_ABS(yaw_angle) > 50)
        {
            IMU_Yaw_Angle_Get_Control(RESET);
            Distance_Reset();
            obstacle_flag = OBSTACLE_TURN_LEFT_PRE;
        }
        break;

    case OBSTACLE_TURN_LEFT_PRE:
        position = 76;
        Distance_Calculation();
        if (car_distance_cm > 40)
        {
            Distance_Reset();
            obstacle_flag = OBSTACLE_STRAIGHT;
        }
        break;

    case OBSTACLE_TURN_LEFT_BACK:
        position = 76;
        IMU_Yaw_Angle_Get_Control(ON);
        Distance_Calculation();
        if (FUNC_ABS(yaw_angle) > 50)
        {
            IMU_Yaw_Angle_Get_Control(RESET);
            Distance_Reset();
            BEEP_OFF();
            obstacle_flag = OBSTACLE_STRAIGHT;
        }
        break;

    case OBSTACLE_STRAIGHT:
        position = 0;
        Distance_Calculation();
        if (car_distance_cm > 50)
        {
            Distance_Reset();
            obstacle_flag = ELEMENT_NONE;
        }
        break;

    case OBSTACLE_DEBUG_STOP:
        speed.normal = 0;
        position = 0;
        break;

    case OBSTACLE_NONE:
        element_busy_flag = ELEMENT_NONE;
        break;

    default:
        break;
    }
}