#include "pid.h"

PIDTypeDef direction, motor_left, motor_right;

float direction_output = 0;
int16 position_last = 0;

int16 motor_left_error, motor_right_error = 0;
int16 motor_left_last_error, motor_right_last_error = 0;
int16 target_speed = 70, target_speed_left = 0, target_speed_right = 0;

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
    PID_Parameter_Init(&direction, 0, 0.0, 0.0, 0.0, 0.0);
    // PID_Parameter_Init(&motor_left, 1.8, 2.0, 0.0, 0.0, 0.0); //+-5
    // PID_Parameter_Init(&motor_right, 2.0, 2.5, 0.0, 0.0, 0.0);

    PID_Parameter_Init(&motor_left, 2.0, 2.2, 0.0, 0.0, 0.0); //0.9 0.22
    PID_Parameter_Init(&motor_right, 2.0, 2.2, 0.0, 0.0, 0.0);
}

void PID_Init(void)
{
    PIDType_Init();
}

void PID_Process(void)
{
    Direction_PID();
    Left_Speed_PID();
    Right_Speed_PID();
}

// 串级PID
//  转向内环 PD 二次项 PD
void Direction_PID(void)
{
    direction_output = position * direction.KP + (position - position_last) * direction.KD;
    // + abs(position) * position * direction.KP_2 + gyro_z_filtered * direction.KD_2; //加入二次项，转向更迅速，直道灵敏度降低。融合陀螺仪，转向增加阻尼，更平稳。

    position_last = position;

    target_speed_left = target_speed + direction_output;
    target_speed_right = target_speed - direction_output;
}

int16 left_speed_I = 0;
// 左轮外环 PI 级
void Left_Speed_PID(void)
{
    motor_left_error = (int16)(target_speed_left - encoder_left.encoder_now);

    left_speed_I = motor_left_error * motor_left.KI;
    left_speed_I = FUNC_LIMIT(left_speed_I,200); //I限幅

    motor_left_pwm += (motor_left_error - motor_left_last_error) * motor_left.KP + left_speed_I;    
    motor_left_last_error = motor_left_error;
}

// 右轮外环 PI
void Right_Speed_PID(void)
{
    motor_right_error = (int16)(target_speed_right - encoder_right.encoder_now);
    motor_right_pwm += (motor_right_error - motor_right_last_error) * motor_right.KP + motor_right_error * motor_right.KI;
    motor_right_last_error = motor_right_error;
}

/*************************************并级PID

// 转向 PD 二次项 PD
void Direction_PID(void)
{
    direction_output = position * direction.KP + (position - position_last) * direction.KD;
    // + abs(position) * position * direction.KP_2 + gyro_z_filtered * direction.KD_2; //加入二次项，转向更迅速，直道灵敏度降低。融合陀螺仪，转向增加阻尼，更平稳。
    position_last = position;
}

// 左轮内环 PI
void Left_Speed_PID(void)
{
    motor_left_error = (int16)(target_speed - encoder_left.encoder_now);
    motor_left_pwm += (motor_left_error - motor_left_last_error) * motor_left.KP + motor_left_error * motor_left.KI;
    motor_left_last_error = motor_left_error;

    motor_left_pwm -= direction_output; // 融合方向控制
}

// 右轮内环 PI
void Right_Speed_PID(void)
{
    motor_right_error = (int16)(target_speed - encoder_right.encoder_now);
    motor_right_pwm += (motor_right_error - motor_right_last_error) * motor_right.KP + motor_right_error * motor_right.KI;
    motor_right_last_error = motor_right_error;

    motor_right_pwm += direction_output; // 融合方向控制
}

*/