#include "pid.h"

PIDTypeDef direction, motor_left, motor_right;

float direction_output;
int16 position_last;

int motor_left_error, motor_right_error;
int motor_left_last_error, motor_right_last_error;
int target_speed_left, target_speed_right;

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
    PID_Parameter_Init(&direction, 0.0, 0.0, 0.0, 0.0, 0.0);
    PID_Parameter_Init(&motor_left, 0.0, 0.0, 0.0, 0.0, 0.0);
    PID_Parameter_Init(&motor_right, 0.0, 0.0, 0.0, 0.0, 0.0);
}

void PID_Init(void)
{
    PIDType_Init();
    EncoderType_Init();
}

void PID_Process(void)
{
    Read_Encoder();
    Direction_PID();
    Left_Speed_PID();
    Right_Speed_PID();
}

// PID转向控制
void Direction_PID(void)
{
    direction_output = position * direction.KP + (position - position_last) * direction.KD; //+ abs(position) * position * direction.KP_2 + groy_z * KD2; //加入二次项，转向更迅速，直道灵敏度降低。融合陀螺仪，转向增加阻尼，更平稳。
    position_last = position;
}

// 左轮内环
void Left_Speed_PID(void)
{
    motor_left_error = (int)(target_speed_left - encoder_left.encoder_now);
    motor_left_pwm += (motor_left_error - motor_left_last_error) * motor_left.KP + motor_left_error * motor_left.KI;
    motor_left_last_error = motor_left_error;

    motor_left_pwm -= direction_output; // 融合方向控制
    motor_left_pwm = clamp(motor_left_pwm, -3500, 8000); //PWM限幅
}

// 右轮内环
void Right_Speed_PID(void)
{
    motor_right_error = (int)(target_speed_right - encoder_right.encoder_now);
    motor_right_pwm += (motor_right_error - motor_right_last_error) * motor_right.KP + motor_right_error * motor_left.KI;
    motor_right_last_error = motor_right_error;

    motor_right_pwm += direction_output; // 融合方向控制
    motor_right_pwm = clamp(motor_right_pwm, -3500, 8000); //PWM限幅
}

int clamp(int value, int min_value, int max_value)
{
    if (value < min_value)
    {
        return min_value;
    }
    else if (value > max_value)
    {
        return max_value;
    }
    else
    {
        return value;
    }
}
