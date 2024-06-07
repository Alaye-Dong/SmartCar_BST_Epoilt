#include "pid.h"

PIDTypeDef direction, motor_left, motor_right;

int16 position_delta_error = 0; // 用于方向环模糊PID的偏差变化量
float direction_output = 0;
int16 position_last = 0;

int16 motor_left_error = 0, motor_right_error = 0;
int16 motor_left_last_error = 0, motor_right_last_error = 0;

uint8 direction_pid_time_flag = 0; // 方向环控制周期标志位 （20ms

// 70速减速系数0.004 80速0.005
SpeedTypeDef speed = {80, 0, 0.004,
                      0, 0, 0};

void PID_Parameter_Init(PIDTypeDef *sptr, float KP, float KI, float KD, float KP_2, float KD_2, float KF)
{
    sptr->KP = KP;
    sptr->KI = KI;
    sptr->KD = KD;
    sptr->KP_2 = KP_2;
    sptr->KD_2 = KD_2;
    sptr->KF = KF;
}

// TODO 调节合适的前馈系数
void PIDType_Init(void)
{
    // PID_Parameter_Init(&direction, 0.45, 0.0, 1.2, 0.001, 0.002, 0); // 50速
    PID_Parameter_Init(&direction, 0.5, 0, 1.2, 0.001, 0.006, 0.1); // 70速 // * 如果使用方向环模糊PID，此处参数设置将无效

    PID_Parameter_Init(&motor_left, 27.33, 2.737, 0, 0, 0, 0.0); // 一般固定速度P
    PID_Parameter_Init(&motor_right, 30.28, 3.818, 0, 0, 0, 0.0);

    // // *使用动态P
    // PID_Parameter_Init(&direction, 0.47, 0.0, 1.2, 0.001, 0.003, 0); // 50速
    // PID_Parameter_Init(&motor_left, 0, 2.737, 0.0, 0.0, 0.0, 0); // 初始为0，使用动态P
    // PID_Parameter_Init(&motor_right, 0, 3.818, 0.0, 0.0, 0.0, 0);
}

void PID_Init(void)
{
    PIDType_Init();
}

#define DIRECTION_PID_PERIOD 4 // 定义20ms的周期，即4个5ms周期
void PID_Process(void)
{
    Position_Loss_Remember();

    if (direction_pid_time_flag != DIRECTION_PID_PERIOD - 1) // 方向环控制周期为20ms，即3次5ms中断标志位后，再下一次中断时即20ms
    {
        direction_pid_time_flag++;
    }
    else
    {
        direction_pid_time_flag = 0; // 重置周期计数器

        Direction_PID();
    }

    Left_Speed_PID();
    Right_Speed_PID();
}

// 丢线记忆打角
void Position_Loss_Remember(void)
{
    if ((inductor[LEFT_V] == 0 && inductor[RIGHT_V] == 0) || (inductor[LEFT_H] <= 5 && inductor[RIGHT_H] <= 5)) // 丢线阈值记忆打角
    {
        position = position_last;
    }
}

// *******************************串级PID 偏差->>转向环->>速度环->>PWM
//  转向外环 PD 二次项 PD
void Direction_PID(void)
{
    position_delta_error = position - position_last;                                                                   // 计算误差变化量
    Fuzzy_PID_Control(position, position_delta_error, &direction.KP, &direction.KD, &direction.KP_2, &direction.KD_2); // 模糊PID计算赋值

    // 加入二次项，转向更迅速，直道灵敏度降低。融合陀螺仪，转向增加阻尼，更平稳。
    direction_output = position * direction.KP + (position - position_last) * direction.KD + abs(position) * position * direction.KP_2 - gyro_z_filtered * direction.KD_2;
    direction_output += position_delta_error * direction.KF; // 合并前馈量
    position_last = position;

    speed.target_left = speed.target - direction_output;
    speed.target_right = speed.target + direction_output;
}

// 左轮内环 PI
void Left_Speed_PID(void)
{
    motor_left_error = (int16)(speed.target_left - encoder_left.encoder_filtered);
    // motor_left.KP = Increment_PI_Dynamic_P_MAX(speed.target_left, encoder_left.encoder_filtered); // 使用增量式动态P
    motor_left_pwm += (motor_left_error - motor_left_last_error) * motor_left.KP + motor_left_error * motor_left.KI;
    motor_left_pwm += motor_left_error * motor_left.KF; // 合并前馈量

    motor_left_last_error = motor_left_error;
}

// 右轮内环 PI
void Right_Speed_PID(void)
{
    motor_right_error = (int16)(speed.target_right - encoder_right.encoder_filtered);
    // motor_right.KP = Increment_PI_Dynamic_P_MAX(speed.target_right, encoder_right.encoder_filtered); // 使用增量式动态P
    motor_right_pwm += (motor_right_error - motor_right_last_error) * motor_right.KP + motor_right_error * motor_right.KI;
    motor_right_pwm += motor_right_error * motor_right.KF; // 合并前馈量

    motor_right_last_error = motor_right_error;
}

// 速度控制，弯道减速
void Speed_Contrl(void)
{
    // speed.target = speed.normal;
    // speed.target = speed.normal - FUNC_ABS(position * speed.deceleration_factor);

    // TODO 利用陀螺仪进行直道弯道判断
    speed.target = speed.normal - FUNC_ABS(gyro_z_filtered * speed.deceleration_factor); // 速度控制，弯道减速

    //////////**************直道***********/
    ////////////
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

// // *************************************并级PID

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

/**
 * 动态调整增量PI控制的P参数的函数,计算出的P为理论上最优激活灵敏度最大值
 * 本函数用于根据编码器增量实现对P参数的动态调整
 *
 * @param encoder_now 上一周期的实际速度（int16类型）
 * @param encoder_increment 增域，即编码器增量（int16类型）：（编码器读值与目标值的差）encoder_target - encoder_now
 * @param ENCODER_MAX 限幅值
 * @return 返回P参数的调整值（float类型）。如果编码器增量为0，则返回一个一般性的P值（30）。
 */
#define ENCODER_MAX 100
float Increment_PI_Dynamic_P_MAX(int16 target_speed_encoder, int16 encoder_now)
{
    int16 encoder_increment = target_speed_encoder - encoder_now;
    if (encoder_increment == 0) // 分母不能为0
    {
        return 30; // 返回一个一般性的P
    }

    return FUNC_ABS((float)(ENCODER_MAX - encoder_now) / encoder_increment);
}

// *********************************************模糊PID

// 定义模糊集合
#define NB -3 // Negative Big
#define NM -2 // Negative Medium
#define NS -1 // Negative Small
#define Z 0   // Zero
#define PS 1  // Positive Small
#define PM 2  // Positive Medium
#define PB 3  // Positive Big

// 模糊化函数，将偏差和偏差变化率转换为模糊集合
int Fuzzify(float value)
{
    if (value <= -75)
        return NB;
    else if (value <= -50)
        return NM;
    else if (value <= -25)
        return NS;
    else if (value <= 25)
        return Z;
    else if (value <= 50)
        return PS;
    else if (value <= 75)
        return PM;
    else
        return PB;
}

// 模糊规则库
int Fuzzy_Rule(int error, int delta_error)
{
    if (error == NB && delta_error == NB)
        return PB;
    else if (error == NB && delta_error == NM)
        return PB;
    else if (error == NB && delta_error == NS)
        return PM;
    else if (error == NB && delta_error == Z)
        return PM;
    else if (error == NB && delta_error == PS)
        return PS;
    else if (error == NB && delta_error == PM)
        return Z;
    else if (error == NB && delta_error == PB)
        return Z;

    else if (error == NM && delta_error == NB)
        return PB;
    else if (error == NM && delta_error == NM)
        return PM;
    else if (error == NM && delta_error == NS)
        return PM;
    else if (error == NM && delta_error == Z)
        return PS;
    else if (error == NM && delta_error == PS)
        return Z;
    else if (error == NM && delta_error == PM)
        return Z;
    else if (error == NM && delta_error == PB)
        return NS;

    else if (error == NS && delta_error == NB)
        return PM;
    else if (error == NS && delta_error == NM)
        return PM;
    else if (error == NS && delta_error == NS)
        return PS;
    else if (error == NS && delta_error == Z)
        return Z;
    else if (error == NS && delta_error == PS)
        return Z;
    else if (error == NS && delta_error == PM)
        return NS;
    else if (error == NS && delta_error == PB)
        return NM;

    else if (error == Z && delta_error == NB)
        return PM;
    else if (error == Z && delta_error == NM)
        return PS;
    else if (error == Z && delta_error == NS)
        return Z;
    else if (error == Z && delta_error == Z)
        return Z;
    else if (error == Z && delta_error == PS)
        return Z;
    else if (error == Z && delta_error == PM)
        return NS;
    else if (error == Z && delta_error == PB)
        return NM;

    else if (error == PS && delta_error == NB)
        return PS;
    else if (error == PS && delta_error == NM)
        return Z;
    else if (error == PS && delta_error == NS)
        return Z;
    else if (error == PS && delta_error == Z)
        return NS;
    else if (error == PS && delta_error == PS)
        return NM;
    else if (error == PS && delta_error == PM)
        return NM;
    else if (error == PS && delta_error == PB)
        return NB;

    else if (error == PM && delta_error == NB)
        return Z;
    else if (error == PM && delta_error == NM)
        return NS;
    else if (error == PM && delta_error == NS)
        return NM;
    else if (error == PM && delta_error == Z)
        return NM;
    else if (error == PM && delta_error == PS)
        return NB;
    else if (error == PM && delta_error == PM)
        return NB;
    else if (error == PM && delta_error == PB)
        return NB;

    else if (error == PB && delta_error == NB)
        return NS;
    else if (error == PB && delta_error == NM)
        return NM;
    else if (error == PB && delta_error == NS)
        return NM;
    else if (error == PB && delta_error == Z)
        return NB;
    else if (error == PB && delta_error == PS)
        return NB;
    else if (error == PB && delta_error == PM)
        return NB;
    else if (error == PB && delta_error == PB)
        return NB;

    return Z;
}

// 模糊 PID 控制器
void Fuzzy_PID_Control(float error, float delta_error, float *kp, float *kd, float *kp2, float *kd2)
{
    int fuzzy_error = Fuzzify(error);             // 模糊化误差
    int fuzzy_delta_error = Fuzzify(delta_error); // 模糊化误差变化率

    int fuzzy_output = Fuzzy_Rule(fuzzy_error, fuzzy_delta_error);

    if (speed.normal <= 70)
    {
        // 根据模糊输出调整 PID 参数
        switch (fuzzy_output)
        {
        case NB:
        case PB: // 因为车左右转向是完全对称，所以这里可以将两种模糊结果得同一个值
            *kp = 0.52;
            *kd = 1.25;
            *kp2 = 0.002;
            *kd2 = 0.002;
            break;
        case NM:
        case PM:
            *kp = 0.48;
            *kd = 1.2;
            *kp2 = 0.0015;
            *kd2 = 0.003;
            break;
        case NS:
        case PS:
            *kp = 0.45;
            *kd = 1.2;
            *kp2 = 0.001;
            *kd2 = 0.005;
            break;
        case Z:
            *kp = 0.42;
            *kd = 1.2;
            *kp2 = 0.001;
            *kd2 = 0.006;
            break;
        }
    }
    else if (speed.normal <= 80)
    {
        // 根据模糊输出调整 PID 参数
        switch (fuzzy_output)
        {
        case NB:
        case PB: // 因为车左右转向是完全对称，所以这里可以将两种模糊结果得同一个值
            *kp = 0.50;
            *kd = 1.2;
            *kp2 = 0.002;
            *kd2 = 0.002;
            break;
        case NM:
        case PM:
            *kp = 0.47;
            *kd = 1.2;
            *kp2 = 0.0015;
            *kd2 = 0.003;
            break;
        case NS:
        case PS:
            *kp = 0.45;
            *kd = 1.2;
            *kp2 = 0.001;
            *kd2 = 0.005;
            break;
        case Z:
            *kp = 0.42;
            *kd = 1.2;
            *kp2 = 0.001;
            *kd2 = 0.006;
            break;
        }
    }
}
