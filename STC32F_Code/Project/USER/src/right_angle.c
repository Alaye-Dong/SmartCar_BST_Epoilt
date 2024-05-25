#include "right_angle.h"

int8 right_angle_flag = RIGHT_ANGLE_NONE;

// 直角元素识别
void Right_Angle_Recognition(void)
{
    if (inductor_left_H <= 10 && inductor_right_H <= 10)
    {
        if (inductor_left_V >= 80)
        {
            right_angle_flag = RIGHT_ANGLE_TURN_LEFT;
        }
        else if (inductor_right_V >= 80)
        {
            right_angle_flag = RIGHT_ANGLE_TURN_RIGHT;
        }
    }
}

void Right_Angle_Turn_Process(void)
{
    if (right_angle_flag == RIGHT_ANGLE_TURN_LEFT)
    {
        position = -50;
        IMU_Yaw_Angle_Get_Control(ON);
        if (yaw_angle >= 90)
        {
            right_angle_flag = RIGHT_ANGLE_NONE;
            IMU_Yaw_Angle_Get_Control(RESET);
        }
    }
    else if (right_angle_flag == RIGHT_ANGLE_TURN_RIGHT)
    {
        position = 50;
        IMU_Yaw_Angle_Get_Control(ON);
        if (yaw_angle <= -90)
        {
            right_angle_flag = RIGHT_ANGLE_NONE;
            IMU_Yaw_Angle_Get_Control(RESET);
        }
    }
}