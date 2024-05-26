#include "round.h"

ROUND_STATE_enmu round_flag = ROUND_NONE;
//float round_right_knn = 0;

int16 round_right_flag_target[INDUCTORS] = {18, 18, 70, 66}; //顺序必须与inductor[LEFT_V] inductor[RIGHT_V] inductor[LEFT_H] inductor[RIGHT_H]对应
float round_right_proximity_coeff = 0; //右环岛接近系数 （1时符合完全目标）
void Round_Recognition(void)
{
    // round_right_knn = Knn_Calculation(18,18,70,66);
    // round_right_knn = pow(18.0 - inductor[LEFT_V], 2) + pow(18.0 - inductor[RIGHT_V], 2) + pow(70.0 - inductor[LEFT_H], 2) + pow(66.0 - inductor[RIGHT_H], 2);
    // round_right_knn = (18.0 - inductor[LEFT_V]) + (18.0 - inductor[RIGHT_V]) + (70.0 - inductor[LEFT_H]) + (66.0 - inductor[RIGHT_H]);
    round_right_proximity_coeff = Proximity_Coefficient(inductor, round_right_flag_target, INDUCTORS);


}

void Round_Turn_Process(void)
{
    // switch (round_flag)
    // {
    // case ROUND_LEFT_IN:

    //     if (inductor[LEFT_H] >= 15 && inductor[RIGHT_H] >= 15)
    //     {
    //         round_flag = ROUND_LEFT_OUT;
    //     }
    //     break;
    // case ROUND_LEFT_OUT:

    //     if (inductor[LEFT_H] <= 15 && inductor[RIGHT_H] <= 15)
    // }
}