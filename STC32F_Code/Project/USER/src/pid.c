#include "pid.h"

PIDTypeDef direction, motor_left, motor_right;

float direction_output = 0;
int16 position_last = 0;

int16 motor_left_error = 0, motor_right_error = 0;
int16 motor_left_last_error = 0, motor_right_last_error = 0;
int16 target_speed = 50, target_speed_left = 0, target_speed_right = 0;

uint8 direction_pid_time_flag = 0; // ���򻷿������ڱ�־λ ��20ms

void PID_Parameter_Init(PIDTypeDef *sptr, float KP, float KI, float KD, float KP_2, float KD_2)
{
    sptr->KP = KP;
    sptr->KI = KI;
    sptr->KD = KD;
    sptr->KP_2 = KP_2;
    sptr->KD_2 = KD_2;
}

void PIDType_Init(void)
{
    PID_Parameter_Init(&direction, 0.45, 0.0, 1.2, 0.001, 0.002); // 50�� D1.2

    PID_Parameter_Init(&motor_left, 27.33, 2.737, 0.0, 0.0, 0.0);
    PID_Parameter_Init(&motor_right, 30.28, 3.818, 0.0, 0.0, 0.0);
}

void PID_Init(void)
{
    PIDType_Init();
}

void PID_Process(void)
{
    if (direction_pid_time_flag != 3) // ���򻷿�������Ϊ20ms����3��5ms�жϱ�־λ������һ���ж�ʱ��20ms
    {
        direction_pid_time_flag++;
    }
    else
    {
        direction_pid_time_flag = 0;

        Direction_PID();
    }

    Left_Speed_PID();
    Right_Speed_PID();
}

// *******************************����PID
//  ת���ڻ� PD ������ PD
void Direction_PID(void)
{
    direction_output = position * direction.KP + (position - position_last) * direction.KD + abs(position) * position * direction.KP_2 - gyro_z_filtered * direction.KD_2;
    // ��������ת���Ѹ�٣�ֱ�������Ƚ��͡��ں������ǣ�ת���������ᣬ��ƽ�ȡ�

    position_last = position;

    target_speed_left = target_speed - direction_output;
    target_speed_right = target_speed + direction_output;
}

// �����⻷ PI
void Left_Speed_PID(void)
{
    motor_left_error = (int16)(target_speed_left - encoder_left.encoder_filtered);
    motor_left_pwm += (motor_left_error - motor_left_last_error) * motor_left.KP + motor_left_error * motor_left.KI;
    motor_left_last_error = motor_left_error;
}

// �����⻷ PI
void Right_Speed_PID(void)
{
    motor_right_error = (int16)(target_speed_right - encoder_right.encoder_filtered);
    motor_right_pwm += (motor_right_error - motor_right_last_error) * motor_right.KP + motor_right_error * motor_right.KI;
    motor_right_last_error = motor_right_error;
}

//*************************************����PID

// // ת�� PD ������ PD
// void Direction_PID(void)
// {
//     direction_output = position * direction.KP + (position - position_last) * direction.KD;
//     // + abs(position) * position * direction.KP_2 + gyro_z_filtered * direction.KD_2; //��������ת���Ѹ�٣�ֱ�������Ƚ��͡��ں������ǣ�ת���������ᣬ��ƽ�ȡ�
//     position_last = position;
// }

// // �����ڻ� PI
// void Left_Speed_PID(void)
// {
//     motor_left_error = (int16)(target_speed - encoder_left.encoder_now);
//     motor_left_pwm += (motor_left_error - motor_left_last_error) * motor_left.KP + motor_left_error * motor_left.KI;
//     motor_left_last_error = motor_left_error;

//     motor_left_pwm -= direction_output; // �ںϷ������
// }

// // �����ڻ� PI
// void Right_Speed_PID(void)
// {
//     motor_right_error = (int16)(target_speed - encoder_right.encoder_now);
//     motor_right_pwm += (motor_right_error - motor_right_last_error) * motor_right.KP + motor_right_error * motor_right.KI;
//     motor_right_last_error = motor_right_error;

//     motor_right_pwm += direction_output; // �ںϷ������
// }
