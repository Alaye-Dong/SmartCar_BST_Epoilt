#include "round.h"

ROUND_STATE_enmu round_flag = ROUND_NONE;

float round_left_proximity_coeff = 0;
float round_right_proximity_coeff = 0; // �����ӽ�ϵ�� ��0ʱ������ȫĿ�꣩

const int16 round_left_target_inductor[4] = {6, 16, 53, 51}; // ����Ԫ��˳������inductor�е�˳���Ӧ
const int16 round_right_target_inductor[4] = {15, 9, 53, 57};

void Round_Recognition(void)
{
    // round_left_proximity_coeff = Calculate_Minkowski_Distance(inductor, round_left_target_inductor, 4, 1);
    // round_right_proximity_coeff = Calculate_Minkowski_Distance(inductor, round_right_target_inductor, 4, 1);

    if ((inductor[LEFT_H] + inductor[RIGHT_H] > 90) && (inductor[LEFT_H] + inductor[RIGHT_H] < 120))
    {
        if (inductor[LEFT_V] + inductor[RIGHT_V] < 40)
        {
            Beep_Buzzing(5, 3);
            round_flag = ROUND_PRE;
        }
    }
}

enum Round_Type
{
    ROUND_TYPE_NONE = 0,
    ROUND_TYPE_LEFT,
    ROUND_TYPE_RIGHT
};

void Round_Turn_Process(void)
{
    uint8 temp_round_type = ROUND_TYPE_NONE;

    element_busy_flag = 1;

    switch (round_flag)
    {
    case ROUND_PRE:
        position *= 0.2;

        // Round_Type_Manual_Deal(); //  �ֶ�ָ�������ʹ��� // TODO �Զ�ʶ�����ͺ�ָ�������͵�ʹ����һ����ѡ����
        Round_Type_Auto_Deal();
        break;

    // ***** �� ********************************************
    case ROUND_LEFT_PRE:
        position *= 0.3;
        if (inductor[RIGHT_V] < 5 && inductor[RIGHT_V] - inductor[LEFT_V] > 30)
        {
            round_flag = ROUND_LEFT_TURN_IN;
        }

        break;

    case ROUND_LEFT_TURN_IN:
        BEEP_ON();
        position += 40;
        IMU_Yaw_Angle_Get_Control(ON);
        if (FUNC_ABS(yaw_angle) >= 25)
        {
            round_flag = ROUND_LEFT_IN;
        }
        break;

    case ROUND_LEFT_IN:
        IMU_Yaw_Angle_Get_Control(ON);
        if (FUNC_ABS(yaw_angle) >= 280)
        {
            round_flag = ROUND_LEFT_TURN_OUT;
            BEEP_OFF();
        }
        break;

    case ROUND_LEFT_TURN_OUT:
        LED = 0;
        position += 40;
        IMU_Yaw_Angle_Get_Control(ON);
        if (FUNC_ABS(yaw_angle) >= 340)
        {
            round_flag = ROUND_LEFT_OUT;
        }
        break;

    case ROUND_LEFT_OUT:
        if (car_distance_real_cm < 100)
        {
            position *= 0.3;
        }

        Distance_Calculation();
        if (car_distance_real_cm > 120)
        {
            Distance_Reset();
            round_flag = ROUND_NONE;
            IMU_Yaw_Angle_Get_Control(RESET);
            element_busy_flag = 0;
            LED = 1;
            BEEP_OFF();
        }
        break;

    // ************************************* �һ� ********
    case ROUND_RIGHT_PRE:
        position *= 0.3;

        if (inductor[LEFT_V] < 5 && inductor[RIGHT_V] - inductor[LEFT_V] > 30)
        {
            round_flag = ROUND_RIGHT_TURN_IN;
        }

        break;

    case ROUND_RIGHT_TURN_IN:
        LED = 0;
        position -= 30;
        IMU_Yaw_Angle_Get_Control(ON);
        if (FUNC_ABS(yaw_angle) >= 25)
        {
            round_flag = ROUND_RIGHT_IN;
        }
        break;

    case ROUND_RIGHT_IN:
        BEEP_ON();
        IMU_Yaw_Angle_Get_Control(ON);
        if (FUNC_ABS(yaw_angle) >= 280)
        {
            round_flag = ROUND_RIGHT_TURN_OUT;
            BEEP_OFF();
        }
        break;

    case ROUND_RIGHT_TURN_OUT:
        LED = 1;
        position -= 30;
        IMU_Yaw_Angle_Get_Control(ON);
        if (FUNC_ABS(yaw_angle) >= 340)
        {
            IMU_Yaw_Angle_Get_Control(RESET);
            round_flag = ROUND_RIGHT_OUT;
        }
        break;

    case ROUND_RIGHT_OUT:
        Distance_Calculation();
        if (car_distance_real_cm > 120)
        {
            Distance_Reset();
            round_flag = ROUND_NONE;
        }
        break;

    case ROUND_NONE:
        element_busy_flag = 0;
        BEEP_OFF();
        break;

    default:
        break;
    }
}

void Round_Type_Auto_Deal(void)
{
    static uint8 round_type = ROUND_TYPE_NONE;

    Distance_Calculation();
    if ((inductor[LEFT_H] + inductor[RIGHT_H] > 130) && (inductor[LEFT_H] > 70))
    {
        round_type = ROUND_TYPE_LEFT;
    }
    else if ((inductor[RIGHT_H] + inductor[LEFT_H] > 130) && (inductor[RIGHT_H] > 70))
    {
        round_type = ROUND_TYPE_RIGHT;
    }

    if (round_type != ROUND_TYPE_NONE && car_distance_real_cm < 100)
    {
        switch (round_type)
        {
        case ROUND_TYPE_LEFT:
            round_flag = ROUND_LEFT_PRE;
            break;
        case ROUND_TYPE_RIGHT:
            round_flag = ROUND_RIGHT_PRE;
            break;

        case ROUND_TYPE_NONE:
            round_flag = ROUND_NONE; // Ԥ�뻷����û����ȷ�뻷���˳���Ԫ�ش���
            break;
        }
        Distance_Reset();
    }
}

#define ROUND_NUMBER_MAX 2

uint8 rounds_type[ROUND_NUMBER_MAX] = {ROUND_TYPE_LEFT, ROUND_TYPE_RIGHT}; // TODO �޸�round_type����ֵ�ķ���
uint8 round_number = 0;
void Round_Type_Manual_Deal(void)
{
    if (round_number >= 0 && round_number < ROUND_NUMBER_MAX) // ȷ��round_number����Ч��Χ��
    {
        switch (rounds_type[round_number])
        {
        case ROUND_TYPE_LEFT:
            round_flag = ROUND_LEFT_PRE;
            break;
        case ROUND_TYPE_RIGHT:
            round_flag = ROUND_RIGHT_PRE;
            break;
        case ROUND_TYPE_NONE:
            round_flag = ROUND_NONE;
            break;
        }
    }
    else
    {
        // ���round_number������Χ����������һ��Ĭ����Ϊ���ߴ�����
        round_flag = ROUND_NONE; // �����������ʵĴ���ʽ
    }
    round_number++; // �б�һ�κ�����Բ��������
}