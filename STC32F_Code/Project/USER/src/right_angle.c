#include "right_angle.h"

RIGHT_ANGLE_STATE_enmu right_angle_flag = RIGHT_ANGLE_NONE;

// 直角元素识别
void Right_Angle_Recognition(void)
{
    if (inductor_left_H <= 15 || inductor_right_H <= 15)
    {
        if (inductor_left_V >= 90 && inductor_right_V <= 20)
        {
            right_angle_flag = RIGHT_ANGLE_TURN_LEFT;
        }
        else if (inductor_right_V >= 90 && inductor_left_V <= 20)
        {
            right_angle_flag = RIGHT_ANGLE_TURN_RIGHT;
        }
    }
}

void Right_Angle_Turn_Process(void)
{
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
        }
    }
}