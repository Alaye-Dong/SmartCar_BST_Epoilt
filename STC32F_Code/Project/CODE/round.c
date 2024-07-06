#include "round.h"

ROUND_STATE_enmu round_flag = ROUND_STATE_NONE;

float round_left_proximity_coeff = 0;
float round_right_proximity_coeff = 0; // 环岛接近系数 （0时符合完全目标）
float round_left_turn_in_proximity_coeff = 0;
float round_right_turn_in_proximity_coeff = 0;

const int16 round_left_target_inductor[INDUCTORS] = {6, 16, 53, 51}; // 数组元素顺序必须和magnet.h中inductor[]的顺序对应
const int16 round_right_target_inductor[INDUCTORS] = {15, 9, 53, 57};
const int16 round_left_turn_in_target_inductor[INDUCTORS] = {15, 15, 100, 75};
const int16 round_right_turn_in_target_inductor[INDUCTORS] = {15, 15, 75, 100};

void Round_Recognition(void)
{
    // round_left_proximity_coeff = Calculate_Minkowski_Distance(inductor, round_left_target_inductor, 4, 1);
    // round_right_proximity_coeff = Calculate_Minkowski_Distance(inductor, round_right_target_inductor, 4, 1);

    if ((inductor[LEFT_H] + inductor[RIGHT_H] > 90) && inductor[LEFT_H] > 30 && inductor[RIGHT_H] > 30)
    {
        if (inductor[LEFT_V] + inductor[RIGHT_V] < 50)
        {
            Beep_Buzzing(5, 3);
            round_flag = ROUND_STATE_PRE;
        }
    }
}

void Round_Proximity_Coeff_Calculate(void)
{
    round_left_turn_in_proximity_coeff = Calculate_Minkowski_Distance(inductor, round_left_turn_in_target_inductor, 4, 1);
    round_right_turn_in_proximity_coeff = Calculate_Minkowski_Distance(inductor, round_right_turn_in_target_inductor, 4, 1);
}

enum Round_Type
{
    ROUND_TYPE_NONE = 0,
    ROUND_TYPE_LEFT,
    ROUND_TYPE_RIGHT
};

void Round_Turn_Process(void)
{
    element_busy_flag = 1;

    switch (round_flag)
    {
    case ROUND_STATE_PRE:
        position *= 0.3;

        // Round_Type_Manual_Deal(); //  手动指定环类型处理 // TODO 自动识别环类型和指定环类型的使用哪一个的选择功能
        Round_Type_Auto_Deal();
        break;

    // ***** 左环 ********************************************
    case ROUND_STATE_LEFT_PRE:
        break;

    case ROUND_STATE_LEFT_TURN_IN:
        BEEP_ON();
        position += 40;
        IMU_Yaw_Angle_Get_Control(ON);
        if (FUNC_ABS(yaw_angle) >= 25)
        {
            round_flag = ROUND_STATE_LEFT_IN;
        }
        break;

    case ROUND_STATE_LEFT_IN:
        IMU_Yaw_Angle_Get_Control(ON);
        if (FUNC_ABS(yaw_angle) >= 280)
        {
            round_flag = ROUND_STATE_LEFT_TURN_OUT;
            BEEP_OFF();
        }
        break;

    case ROUND_STATE_LEFT_TURN_OUT:
        LED = 0;
        position += 40;
        IMU_Yaw_Angle_Get_Control(ON);
        if (FUNC_ABS(yaw_angle) >= 340)
        {
            round_flag = ROUND_STATE_LEFT_OUT;
        }
        break;

    case ROUND_STATE_LEFT_OUT:
        if (car_distance_cm < 100)
        {
            position *= 0.3;
        }

        Distance_Calculation();
        if (car_distance_cm > 120)
        {
            Distance_Reset();
            round_flag = ROUND_STATE_NONE;
            IMU_Yaw_Angle_Get_Control(RESET);
            element_busy_flag = 0;
            LED = 1;
            BEEP_OFF();
        }
        break;

    // ************************************* 右环 ********
    case ROUND_STATE_RIGHT_PRE:

        break;

    case ROUND_STATE_RIGHT_TURN_IN:
        LED = 0;
        position -= 30;
        IMU_Yaw_Angle_Get_Control(ON);
        if (FUNC_ABS(yaw_angle) >= 25)
        {
            round_flag = ROUND_STATE_RIGHT_IN;
        }
        break;

    case ROUND_STATE_RIGHT_IN:
        BEEP_ON();
        IMU_Yaw_Angle_Get_Control(ON);
        if (FUNC_ABS(yaw_angle) >= 280)
        {
            round_flag = ROUND_STATE_RIGHT_TURN_OUT;
            BEEP_OFF();
        }
        break;

    case ROUND_STATE_RIGHT_TURN_OUT:
        LED = 1;
        position -= 30;
        IMU_Yaw_Angle_Get_Control(ON);
        if (FUNC_ABS(yaw_angle) >= 340)
        {
            IMU_Yaw_Angle_Get_Control(RESET);
            round_flag = ROUND_STATE_RIGHT_OUT;
        }
        break;

    case ROUND_STATE_RIGHT_OUT:
        Distance_Calculation();
        if (car_distance_cm > 120)
        {
            Distance_Reset();
            round_flag = ROUND_STATE_NONE;
        }
        break;

    case ROUND_STATE_NONE:
        element_busy_flag = 0;
        BEEP_OFF();
        break;

    default:
        break;
    }
}

/**
 * @description: 使用电感自动判别环岛类型
 * @return {*}
 */
void Round_Type_Auto_Deal(void)
{
    static uint8 round_type = ROUND_TYPE_NONE;
    static uint8 inductor_v_sum_last = 0;

    uint8 inductor_v_sum = 0;

    Distance_Calculation();
    //(inductor[LEFT_V] - inductor[RIGHT_V] > 30)
    if ((inductor[LEFT_H] + inductor[RIGHT_H] > 130) && (inductor[LEFT_H] > 70))
    {
        round_type = ROUND_TYPE_LEFT;
    }
    else if ((inductor[RIGHT_H] + inductor[LEFT_H] > 130) && (inductor[RIGHT_H] > 70))
    {
        round_type = ROUND_TYPE_RIGHT;
    }

    inductor_v_sum = inductor[LEFT_V] + inductor[RIGHT_V];
    // 到达交叉点，使用测出的round_type进行对应打角情况跳转
    //(inductor_v_sum > inductor_v_sum_last) && (inductor[LEFT_H] + inductor[RIGHT_H] > 140)
    if (((round_left_turn_in_proximity_coeff <= 50 || round_right_turn_in_proximity_coeff <= 50) && (inductor[LEFT_H] + inductor[RIGHT_H] > 150)) || // TODO 圆环交叉点电感值阈值
        (car_distance_cm > 200))                                                                                                                     // 超过200cm未检测到交叉点，跳出环元素处理
    {

        switch (round_type)
        {
        case ROUND_TYPE_LEFT:
            round_flag = ROUND_STATE_LEFT_TURN_IN;
            break;
        case ROUND_TYPE_RIGHT:
            round_flag = ROUND_STATE_RIGHT_TURN_IN;
            break;

        case ROUND_TYPE_NONE:
            round_flag = ROUND_STATE_NONE; // 预入环但是没有检测得到打角标志位则退出环元素处理
            break;
        }
        Distance_Reset();
        round_type = ROUND_TYPE_NONE; // 归零，使得下一个圆环检测时正常使用
    }

    inductor_v_sum_last = inductor_v_sum;
}

#define ROUND_NUMBER_MAX 2

uint8 rounds_type[ROUND_NUMBER_MAX] = {ROUND_TYPE_LEFT, ROUND_TYPE_RIGHT}; // TODO 修改round_type数组值的方法
uint8 round_index = 0;

/**
 * @description: 使用人工预设值确定环岛类型
 * @return {*}
 */
void Round_Type_Manual_Deal(void)
{
    if (round_index < ROUND_NUMBER_MAX) // 确保round_number在有效范围内
    {
        switch (rounds_type[round_index])
        {
        case ROUND_TYPE_LEFT:
            round_flag = ROUND_STATE_LEFT_TURN_IN;
            break;
        case ROUND_TYPE_RIGHT:
            round_flag = ROUND_STATE_RIGHT_TURN_IN;
            break;
        case ROUND_TYPE_NONE:
            round_flag = ROUND_STATE_NONE;
            break;
        }
    }

    round_index++; // 判别一次后增加圆环计数器
}