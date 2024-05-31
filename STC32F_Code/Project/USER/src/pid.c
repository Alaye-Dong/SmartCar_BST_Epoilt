#include "pid.h"

PIDTypeDef direction, motor_left, motor_right;

float direction_output = 0;
int16 position_last = 0;

int16 motor_left_error = 0, motor_right_error = 0;
int16 motor_left_last_error = 0, motor_right_last_error = 0;

uint8 direction_pid_time_flag = 0; // 方向环控制周期标志位 （20ms

SpeedTypeDef speed = {70, 0, 0.04,
                      0, 0, 0};

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
    PID_Parameter_Init(&direction, 0.45, 0.0, 1.2, 0.001, 0.002); // 50速
    // 70速 PID_Parameter_Init(&direction, 0.5, 0.0, 1.2, 0.001, 0.006);
    PID_Parameter_Init(&motor_left, 27.33, 2.737, 0.0, 0.0, 0.0);
    PID_Parameter_Init(&motor_right, 30.28, 3.818, 0.0, 0.0, 0.0);
}

void PID_Init(void)
{
    PIDType_Init();
}

void PID_Process(void)
{
    if (direction_pid_time_flag != 3) // 方向环控制周期为20ms，即3次5ms中断标志位后，再下一次中断时即20ms
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

// *******************************串级PID
//  转向内环 PD 二次项 PD
void Direction_PID(void)
{
    direction_output = position * direction.KP + (position - position_last) * direction.KD + abs(position) * position * direction.KP_2 - gyro_z_filtered * direction.KD_2;
    // 加入二次项，转向更迅速，直道灵敏度降低。融合陀螺仪，转向增加阻尼，更平稳。

    position_last = position;

    speed.target_left = speed.target - direction_output;
    speed.target_right = speed.target + direction_output;
}

// 左轮外环 PI
void Left_Speed_PID(void)
{
    motor_left_error = (int16)(speed.target_left - encoder_left.encoder_filtered);
    motor_left_pwm += (motor_left_error - motor_left_last_error) * motor_left.KP + motor_left_error * motor_left.KI;
    motor_left_last_error = motor_left_error;
}

// 右轮外环 PI
void Right_Speed_PID(void)
{
    motor_right_error = (int16)(speed.target_right - encoder_right.encoder_filtered);
    motor_right_pwm += (motor_right_error - motor_right_last_error) * motor_right.KP + motor_right_error * motor_right.KI;
    motor_right_last_error = motor_right_error;
}

// 速度控制，弯道减速
void Speed_Contrl(void)
{
    // speed.target = speed.normal;
    speed.target = speed.normal - FUNC_ABS(position * speed.deceleration_factor);

    //////////**************直道***********/////////////
    // if (GyroX <= 1500 && GyroX >= -1500)
    //     zhidaotime++;
    // if (zhidaotime > 20)
    //     zhidaoflag = 1;
    // if (GyroX > 1500 || GyroX < -1500)
    // {
    //     zhidaotime = 0;
    //     zhidaoflag = 0;
    // }
}

//*************************************并级PID

// // 转向 PD 二次项 PD
// void Direction_PID(void)
// {
//     direction_output = position * direction.KP + (position - position_last) * direction.KD;
//     // + abs(position) * position * direction.KP_2 + gyro_z_filtered * direction.KD_2; //加入二次项，转向更迅速，直道灵敏度降低。融合陀螺仪，转向增加阻尼，更平稳。
//     position_last = position;
// }

// // 左轮内环 PI
// void Left_Speed_PID(void)
// {
//     motor_left_error = (int16)(speed.target - encoder_left.encoder_now);
//     motor_left_pwm += (motor_left_error - motor_left_last_error) * motor_left.KP + motor_left_error * motor_left.KI;
//     motor_left_last_error = motor_left_error;

//     motor_left_pwm -= direction_output; // 融合方向控制
// }

// // 右轮内环 PI
// void Right_Speed_PID(void)
// {
//     motor_right_error = (int16)(speed.target - encoder_right.encoder_now);
//     motor_right_pwm += (motor_right_error - motor_right_last_error) * motor_right.KP + motor_right_error * motor_right.KI;
//     motor_right_last_error = motor_right_error;

//     motor_right_pwm += direction_output; // 融合方向控制
// }
