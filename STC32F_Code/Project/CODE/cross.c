#include "cross.h"

CROSS_STATE_enmu cross_flag = CROSS_NONE;

void Cross_Recognition(void)
{
    if (inductor[LEFT_V] > 90 && inductor[RIGHT_V] > 90)
    {
        cross_flag = CROSS_IN;
    }
}

void Cross_Turn_Process(void)
{
    element_busy_flag = 1;
    switch (cross_flag)
    {
    case CROSS_IN:
        BEEP_ON();
        position = position * 0.3;
        Distance_Calculation();
        if (car_distance_real_cm > 50)
        {
            Distance_Reset();
            cross_flag = CROSS_NONE;
            element_busy_flag = 0;
            BEEP_OFF();
        }
        break;
    default:
        break;
    }
}