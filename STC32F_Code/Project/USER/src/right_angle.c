#include "right_angle.h"

RIGHT_ANGLE_STATE_enmu right_angle_flag = RIGHT_ANGLE_NONE;
float right_angle_left_knn = 0;
float right_angle_right_knn = 0;

// 直角元素识别
void Right_Angle_Recognition(void)
{
    if (inductor[LEFT_H] <= 30 && inductor[RIGHT_H] <= 30)
    {
        if (inductor[LEFT_V] >= 85 && inductor[RIGHT_V] <= 40)
        {
            right_angle_flag = RIGHT_ANGLE_TURN_LEFT;
        }
        else if (inductor[RIGHT_V] >= 85 && inductor[LEFT_V] <= 40)
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
        position = 80;
        IMU_Yaw_Angle_Get_Control(ON);
        if (yaw_angle >= 75)
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
        position = -80;
        IMU_Yaw_Angle_Get_Control(ON);
        if (yaw_angle <= -75)
        {
            right_angle_flag = RIGHT_ANGLE_NONE;
            IMU_Yaw_Angle_Get_Control(RESET);
            BEEP_OFF();

            element_busy_flag = 0;
        }
    }
}