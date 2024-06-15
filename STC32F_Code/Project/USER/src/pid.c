#include "pid.h"

PIDTypeDef direction, motor_left, motor_right;

int16 position_delta_error = 0; // 用于方向环模糊PID的偏差变化量
float direction_output = 0;
int16 position_last = 0;

int16 motor_left_error = 0, motor_right_error = 0;
int16 motor_left_last_error = 0, motor_right_last_error = 0;

uint8 direction_pid_time_counter = 0; // 方向环控制周期标志位 （20ms

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
    PID_Parameter_Init(&direction, 0, 0, 0, 0, 0, 0);
    // PID_Parameter_Init(&direction, 0.45, 0, 2, 0.001, 0.006, 0.1); // 70速 // * 如果使用方向环模糊PID，此处参数设置将无效
    PID_Parameter_Init(&motor_left, 27.33, 2.737, 0, 0, 0, 0.0); // 一般固定速度P
    PID_Parameter_Init(&motor_right, 30.28, 3.818, 0, 0, 0, 0.0);
    // PID_Parameter_Init(&motor_left, 27.33, 5, 0, 0, 0, 0.0); // 一般固定速度P
    // PID_Parameter_Init(&motor_right, 30.28, 5, 0, 0, 0, 0.0);

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
    Speed_Contrl();
    // Position_Loss_Remember();

    if (direction_pid_time_counter != DIRECTION_PID_PERIOD - 1) // 方向环控制周期为20ms，即3次5ms中断标志位后，再下一次中断时即20ms
    {
        direction_pid_time_counter++;
    }
    else
    {
        direction_pid_time_counter = 0; // 重置周期计数器

        Direction_PID();
    }

    Left_Speed_PID();
    Right_Speed_PID();
}

//  丢线记忆打角
uint8 position_loss_flag = 0;
void Position_Loss_Remember(void)
{
    static uint8 position_loss_time_counter = 0;

    if (position_loss_time_counter > 200) // 5 * 200 = 1000ms 丢线累计1s停车保护
    {
        speed.target = 0;
    }

    if ((inductor[LEFT_H] <= 5 && inductor[RIGHT_H] <= 5)) // 短时间丢线，记忆打角
    {
        position = position_last;
        if (position_loss_time_counter < 255) // 防止溢出
        {
            position_loss_time_counter++;
        }
    }
    else // 寻得线，丢线累计时间标志位清零
    {
        position_loss_time_counter = 0;
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
    // direction_output += position_delta_error * direction.KF; // 合并前馈量
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

/* 速度控制，弯道减速 */
uint16 straight_time_flag = 0;
void Speed_Contrl(void)
{
    // TODO 利用陀螺仪进行直道弯道判断
    speed.target = speed.normal - FUNC_ABS(gyro_z_filtered * speed.deceleration_factor); // 速度控制，弯道减速

    // //长直道加速
    // if (FUNC_ABS(gyro_z_filtered) < 500)
    // {
    //     straight_time_flag++;
    // }
    // if (straight_time_flag > 200) // 200 * 5 = 1000ms
    // {
    //     speed.target = speed.target + speed.target * speed.boost_factor;
    // }
    // else
    // {
    //     straight_time_flag = 0;
    // }
}

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

/* 定义模糊子集 */
#define NB 0 // Negative Big
#define NM 1 // Negative Medium
#define NS 2 // Negative Small
#define ZO 3 // Zero
#define PS 4 // Positive Small
#define PM 5 // Positive Medium
#define PB 6 // Positive Big

// 模糊化函数，将偏差和偏差变化率转换为模糊集合
uint8 Fuzzify(float value)
{
    if (value <= -75)
        return NB;
    else if (value <= -50)
        return NM;
    else if (value <= -25)
        return NS;
    else if (value <= 25)
        return ZO;
    else if (value <= 50)
        return PS;
    else if (value <= 75)
        return PM;
    else
        return PB;
}

// 模糊规则表
uint8 fuzzy_rule_table[7][7] = {
    // NB  NM  NS   ZO  PS  PM  PB
    {PB, PB, PB, PB, PM, ZO, ZO}, // NB
    {PB, PB, PB, PB, PM, ZO, ZO}, // NM
    {PB, PM, PM, PS, ZO, NS, NM}, // NS
    {PM, PM, PS, ZO, NM, NM, NM}, //  ZO
    {PS, PS, ZO, NM, NM, NB, PB}, // PS
    {ZO, ZO, ZO, NM, NB, NB, PB}, // PM
    {ZO, NS, NB, NB, NB, NB, NB}  // PB
};

// 使用模糊规则表解模糊
uint8 Fuzzy_Rule(int error, int delta_error)
{
    // 确保错误类型和变化类型在有效范围内
    if (error < NB || error > PB || delta_error < NB || delta_error > PB)
    {
        return ZO; // 默认返回值设为Z，在异常情况下
    }

    return fuzzy_rule_table[error][delta_error];
}

// 模糊 PID 控制器
void Fuzzy_PID_Control(float error, float delta_error, float *kp, float *kd, float *kp2, float *kd2)
{
    int16 fuzzy_error = Fuzzify(error);             // 模糊化误差
    int16 fuzzy_delta_error = Fuzzify(delta_error); // 模糊化误差变化率

    uint8 fuzzy_output = Fuzzy_Rule(fuzzy_error, fuzzy_delta_error);

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
        case ZO:
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
            *kp2 = 0.001;
            *kd2 = 0.001;
            break;
        case NM:
        case PM:
            *kp = 0.47;
            *kd = 1.2;
            *kp2 = 0.001;
            *kd2 = 0.003;
            break;
        case NS:
        case PS:
            *kp = 0.43;
            *kd = 1.2;
            *kp2 = 0.001;
            *kd2 = 0.003;
            break;
        case ZO:
            *kp = 0.40;
            *kd = 1.2;
            *kp2 = 0.001;
            *kd2 = 0.003;
            break;
        }
    }
}
