#include "right_angle.h"

RIGHT_ANGLE_STATE_enmu right_angle_flag = RIGHT_ANGLE_NONE;

// 直角元素识别
void Right_Angle_Recognition(void)
{
    if ((inductor[LEFT_H] >= 15 && inductor[RIGHT_H] >= 15) && (inductor[LEFT_H] + inductor[RIGHT_H] < 80))
    {
        if ((inductor[LEFT_V] >= 30 && inductor[RIGHT_V] <= 20) && (inductor[LEFT_V] - inductor[RIGHT_V] >= 40))
        {
            right_angle_flag = RIGHT_ANGLE_TURN_LEFT;
        }
        else if (inductor[RIGHT_V] >= 30 && inductor[LEFT_V] <= 20 && (inductor[RIGHT_V] - inductor[LEFT_V] >= 40))
        {
            right_angle_flag = RIGHT_ANGLE_TURN_RIGHT;
        }
    }
}

void Right_Angle_Turn_Process(void)
{
    element_busy_flag = 1;
    if (right_angle_flag == RIGHT_ANGLE_TURN_LEFT)
    {
        BEEP_ON();
        position = 200;
        IMU_Yaw_Angle_Get_Control(ON);
        if (yaw_angle >= 80)
        {
            right_angle_flag = RIGHT_ANGLE_NONE;
            IMU_Yaw_Angle_Get_Control(RESET);
            BEEP_OFF();

            element_busy_flag = 0;
        }
    }
    else if (right_angle_flag == RIGHT_ANGLE_TURN_RIGHT)
    {
        BEEP_ON();
        position = -200;
        IMU_Yaw_Angle_Get_Control(ON);
        if (yaw_angle <= -80)
        {
            right_angle_flag = RIGHT_ANGLE_NONE;
            IMU_Yaw_Angle_Get_Control(RESET);
            BEEP_OFF();

            element_busy_flag = 0;
        }
    }
}