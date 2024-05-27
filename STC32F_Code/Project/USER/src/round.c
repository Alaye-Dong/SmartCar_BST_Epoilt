#include "round.h"

ROUND_STATE_enmu round_flag = ROUND_NONE;
// float round_right_knn = 0;

int16 round_right_flag_target[INDUCTORS] = {18, 18, 70, 66}; // 顺序必须与inductor[LEFT_V] inductor[RIGHT_V] inductor[LEFT_H] inductor[RIGHT_H]对应
float round_proximity_coeff = 0;                             // 环岛接近系数 （0时符合完全目标）
void Round_Recognition(void)
{
    // round_right_knn = Knn_Calculation(18,18,70,66);
    // round_right_knn = pow(18.0 - inductor[LEFT_V], 2) + pow(18.0 - inductor[RIGHT_V], 2) + pow(70.0 - inductor[LEFT_H], 2) + pow(66.0 - inductor[RIGHT_H], 2);
    // round_right_knn = (18.0 - inductor[LEFT_V]) + (18.0 - inductor[RIGHT_V]) + (70.0 - inductor[LEFT_H]) + (66.0 - inductor[RIGHT_H]);
    // round_proximity_coeff = Proximity_Coefficient(inductor, round_right_flag_target, INDUCTORS);

    // 曼哈顿距离计算
    round_proximity_coeff = FUNC_ABS(40 - inductor[LEFT_V]) + FUNC_ABS(40 - inductor[RIGHT_V]) + FUNC_ABS(60 - inductor[LEFT_H]) + FUNC_ABS(60 - inductor[RIGHT_H]);

    if (round_proximity_coeff <= 60)
    {
        if (inductor[LEFT_H] - inductor[RIGHT_H] > 10 && inductor[LEFT_V] > inductor[RIGHT_V])
        {
            Beep_Buzzing(5, 3);
            round_flag = ROUND_LEFT_PRE;
        }
        else if (inductor[RIGHT_H] - inductor[LEFT_H] > 10 && inductor[RIGHT_V] > inductor[LEFT_V])
        {
            Beep_Buzzing(5, 3);
            round_flag = ROUND_RIGHT_PRE;
        }
    }
}

void Round_Turn_Process(void)
{
    element_busy_flag = 1;
    switch (round_flag)
    {
    case ROUND_LEFT_PRE:
        position = 0;
        Distance_Calculation();
        if (real_distance > 50)
        {
            Distance_Reset();
            round_flag = ROUND_LEFT_INTO;
        }
        break;
    case ROUND_LEFT_INTO:
        position = 30;
        Distance_Calculation();
        if (real_distance > 20)
        {
            Distance_Reset();
            round_flag = ROUND_LEFT_IN;
        }
        break;
    case ROUND_LEFT_IN:
        BEEP_ON();
        Position_Analyse();
        IMU_Yaw_Angle_Get_Control(ON);
        if (FUNC_ABS(yaw_angle) >= 320)
        {
            round_flag = ROUND_LEFT_OUT;
            IMU_Yaw_Angle_Get_Control(RESET);
            BEEP_OFF();
        }
        break;
    case ROUND_LEFT_OUT:
        round_flag = ROUND_NONE;

        element_busy_flag = 0;
        break;

    case ROUND_RIGHT_PRE:
        position = 0;
        Distance_Calculation();
        if (real_distance > 50)
        {
            Distance_Reset();
            round_flag = ROUND_RIGHT_INTO;
        }

        break;
    case ROUND_RIGHT_INTO:
        position = -30;
        Distance_Calculation();
        if (real_distance > 20)
        {
            Distance_Reset();
            round_flag = ROUND_RIGHT_IN;
        }
    case ROUND_RIGHT_IN:
        LED = 0;
        BEEP_ON();
        Position_Analyse();
        IMU_Yaw_Angle_Get_Control(ON);
        if (FUNC_ABS(yaw_angle) >= 320)
        {
            round_flag = ROUND_RIGHT_OUT;
            IMU_Yaw_Angle_Get_Control(RESET);
            BEEP_OFF();
        }
        break;
    case ROUND_RIGHT_OUT:
        LED = 1;
        round_flag = ROUND_NONE;

        element_busy_flag = 0;
        break;

    default:
        break;
    }
}