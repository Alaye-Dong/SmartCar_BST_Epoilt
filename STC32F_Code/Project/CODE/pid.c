#include "pid.h"

PIDTypeDef direction, motor_left, motor_right;

int16 position_delta_error = 0; // 用于方向环模糊PID的偏差变化量
float direction_output = 0;

SpeedTypeDef speed = {105, 0, 0.01,
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

void PIDType_Init(void)
{
    //  PID_Parameter_Init(&direction, 0, 0, 0, 0, 0, 0);
    //  PID_Parameter_Init(&motor_left, 0, 0, 0, 0, 0, 0);
    //  PID_Parameter_Init(&motor_right, 0, 0, 0, 0, 0, 0);
    PID_Parameter_Init(&direction, 0.5, 0, 6.8, 0.0015, 0.002, 5.6); // * 如果使用方向环模糊PID，此处的P、I、D参数设置将无效
    PID_Parameter_Init(&motor_left, 27.33, 2.737, 0, 0, 0, 0.0);
    PID_Parameter_Init(&motor_right, 30.28, 3.818, 0, 0, 0, 0.0);
}

void PID_Init(void)
{
    PIDType_Init();
}

#define DIRECTION_PID_PERIOD 4 // 定义20ms的周期，即4个5ms周期
// PID串级控制中，外环周期应该大于等于内环周期，使得内环可以有足够的时间响应外环的输入
void PID_Process(void)
{
    static uint8 direction_pid_time_counter = 0; // 方向环控制周期标志位

    if (direction_pid_time_counter != DIRECTION_PID_PERIOD - 1) // 方向环控制周期为20ms，即3次5ms中断标志位后，再下一次中断时即20ms
    {
        direction_pid_time_counter++;
    }
    else
    {
        direction_pid_time_counter = 0; // 重置周期计数器
        Speed_Contrl();
        Direction_PID();
        // Direction_PID_Incomplete_D();
    }

    Left_Speed_PID();
    Right_Speed_PID();
}

// TODO 前馈PID
#define DELTA_DECELERATION_FACTOR 2 // TODO 待调试
void Speed_Contrl(void)
{
    // TODO 利用陀螺仪进行直道弯道判断
    speed.target = speed.normal -
                   FUNC_ABS(gyro_z_filtered * speed.deceleration_factor) -
                   FUNC_ABS(position_delta_error * DELTA_DECELERATION_FACTOR); // 速度控制，弯道减速

    if (speed.target < 0)
    {
        speed.target = 0;
    }

    if (position_loss_stop_protect_flag == 1)
    {
        speed.target = 0; // 丢线停车保护
    }

    // //长直道加速
    // uint16 straight_time_flag = 0;
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

// *******************************串级PID 偏差->>转向环->>角速度环（待实现）->>速度环->>PWM
//  转向外环 PD 二次项 PD
void Direction_PID(void)
{
    static int16 position_last = 0;

    position_delta_error = position - position_last;               // 计算误差变化量
    Fuzzy_PID_Control(position, position_delta_error, &direction); // 模糊PID计算赋值

    direction_output = position * direction.KP + (position - position_last) * direction.KD +
                       abs(position) * position * direction.KP_2 - gyro_z_filtered * direction.KD_2; // 加入二次项，转向更迅速，直道灵敏度降低。融合陀螺仪，转向增加阻尼，更平稳。
    // direction_output += (position_delta_error / (0.005 * DIRECTION_PID_PERIOD) + position)* direction.KF; // 合并前馈量
    position_last = position;

    speed.target_left = speed.target - direction_output;
    speed.target_right = speed.target + direction_output;
}

/* TODO 实验中 不完全微分PID */
#define TF T * 0.05
#define T 0.02
void Direction_PID_Incomplete_D(void)
{
    static int16 position_last = 0;
    static float D_value_last = 0;
    const float alpha = 0.05; // TF / (TF + T);
    float D_value = 0;

    position_delta_error = position - position_last;               // 计算误差变化量
    Fuzzy_PID_Control(position, position_delta_error, &direction); // 模糊PID计算赋值
    D_value = direction.KD * (1 - alpha) * position_delta_error + alpha * D_value_last;
    direction_output = position * direction.KP +
                       D_value +
                       abs(position) * position * direction.KP_2 - gyro_z_filtered * direction.KD_2; // 加入二次项，转向更迅速，直道灵敏度降低。融合陀螺仪，转向增加阻尼，更平稳。

    // D_value = direction.KD * (1 - alpha) * position + alpha * D_value_last - direction.KD * (1 - alpha) * position_last;

    // 更新D值，结合当前位置差和上一D值的加权和

    D_value_last = D_value;
    position_last = position;

    speed.target_left = speed.target - direction_output;
    speed.target_right = speed.target + direction_output;
}

// void ACC_PID(void)
// {
//     static int16 acc_last_error = 0;
//     static float acc_output = 0;
//     int16 acc_error = 0;

//     acc_error = (int16)(direction_output - gyro_z_filtered);

//     acc_output += (acc_error - acc_last_error) * acc.KP + acc_error * acc.KI;

//     acc_last_error = acc_error;

//     speed.target_left = speed.target - acc_output;
//     speed.target_right = speed.target + acc_output;
// }

// 左轮内环 PI
void Left_Speed_PID(void)
{
    static int16 motor_left_last_error = 0;
    int16 motor_left_error = 0;

    motor_left_error = (int16)(speed.target_left - encoder_left.encoder_filtered);
    // motor_left.KP = Increment_PI_Dynamic_P_MAX(speed.target_left, encoder_left.encoder_filtered); // 使用增量式动态P
    motor_left_pwm += (motor_left_error - motor_left_last_error) * motor_left.KP + motor_left_error * motor_left.KI;

    motor_left_last_error = motor_left_error;
}

// 右轮内环 PI
void Right_Speed_PID(void)
{
    static int16 motor_right_last_error = 0;
    int16 motor_right_error = 0;

    motor_right_error = (int16)(speed.target_right - encoder_right.encoder_filtered);
    // motor_right.KP = Increment_PI_Dynamic_P_MAX(speed.target_right, encoder_right.encoder_filtered); // 使用增量式动态P
    motor_right_pwm += (motor_right_error - motor_right_last_error) * motor_right.KP + motor_right_error * motor_right.KI;

    motor_right_last_error = motor_right_error;
}

/**
 * 动态调整增量PI控制的P参数的函数,计算出的P为理论上最优激活灵敏度最大值
 * 本函数用于根据编码器增量实现对P参数的动态调整
 * @note 实测效果不理想，待研究优化
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

#define FUZZY_SET_NUM 7
/* 定义模糊子集 */
#define NB 0 // Negative Big
#define NM 1 // Negative Medium
#define NS 2 // Negative Small
#define ZO 3 // Zero
#define PS 4 // Positive Small
#define PM 5 // Positive Medium
#define PB 6 // Positive Big

// 模糊论域
const int8 fuzzy_domain[FUZZY_SET_NUM] = {-75, -50, -25, 0, 25, 50, 75};

// 最大隶属度法的隶属值匹配表
const int8 fuzzy_domain_maximum_membership_match[FUZZY_SET_NUM] = {(-75 + -50) / 2, (-50 + -25) / 2, (-25 + 0) / 2, (0 + 25) / 2, (25 + 50) / 2, (50 + 75) / 2, (50 + 75) / 2};

// 模糊化函数，将偏差和偏差变化率转换为模糊集合
uint8 Fuzzify(float value)
{
    // 直接映射到最接近的模糊子集
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

    // 隶属度算法
    // uint8 i;
    // for (i = 0; i < FUZZY_SET_NUM - 1; i++)
    // {
    //     if (value <= fuzzy_domain_maximum_membership_match[i])
    //     {
    //         return i;
    //     }
    // }
    // if (value > fuzzy_domain_maximum_membership_match[PB])
    // {
    //     return PB;
    // }
    // return ZO;
}

// 模糊规则表
const uint8 fuzzy_rule_table[FUZZY_SET_NUM][FUZZY_SET_NUM] = {
    // NB NM NS  ZO  PS  PM  PB
    {NB, NB, NM, NM, NS, ZO, ZO}, // NB
    {NB, NM, NM, NS, NS, ZO, PS}, // NM
    {NM, NM, NS, NS, ZO, PS, PS}, // NS
    {NM, NS, NS, ZO, PS, PS, PM}, //  ZO
    {NS, NS, ZO, PS, PS, PM, PM}, // PS
    {NS, ZO, PS, PS, PM, PM, PB}, // PM
    {ZO, ZO, PS, PM, PM, PB, PB}  // PB
};

// 使用模糊规则表解模糊
uint8 Fuzzy_Rule(int fuzzy_error, int fuzzy_delta_error)
{
    // 确保错误类型和变化类型在有效范围内
    if (fuzzy_error < NB || fuzzy_error > PB || fuzzy_delta_error < NB || fuzzy_delta_error > PB)
    {
        return ZO; // 默认返回值设为Z，在异常情况下
    }

    return fuzzy_rule_table[fuzzy_error][fuzzy_delta_error];
}

// 模糊 PID 控制器
void Fuzzy_PID_Control(float error, float delta_error, PIDTypeDef *sptr)
{
    int16 fuzzy_error = Fuzzify(error);             // 模糊化误差
    int16 fuzzy_delta_error = Fuzzify(delta_error); // 模糊化误差变化率

    uint8 fuzzy_output = Fuzzy_Rule(fuzzy_error, fuzzy_delta_error);

    // 根据模糊输出调整 PID 参数
    switch (fuzzy_output)
    {
    case NB:
    case PB: // 因为车左右转向是完全对称，所以这里可以将两种模糊结果得同一个值
        sptr->KP = 1.8;
        sptr->KD = 6.8;
        sptr->KP_2 = 0.006;
        sptr->KD_2 = 0.001;
        break;
    case NM:
    case PM:
        sptr->KP = 1.7;
        sptr->KD = 6.8;
        sptr->KP_2 = 0.006;
        sptr->KD_2 = 0.001;
        break;
    case NS:
    case PS:
        sptr->KP = 0.5;
        sptr->KD = 5.8;
        sptr->KP_2 = 0.001;
        sptr->KD_2 = 0.001;
        break;
    case ZO:
        sptr->KP = 0.4;
        sptr->KD = 5.8;
        sptr->KP_2 = 0.001;
        sptr->KD_2 = 0.001;
        break;
    }
}

/*
TODO 隶属度法解模糊
*/
// // 定义隶属度函数
// float Membership_Function(float x, float x0, float x1, float x2, float x3)
// {
//     if (x <= x0 || x >= x3)
//         return 0.0;
//     else if (x <= x1)
//         return (x - x0) / (x1 - x0);
//     else if (x <= x2)
//         return 1.0;
//     else
//         return (x3 - x) / (x3 - x2);
// }

// // 计算误差和误差变化率的隶属度
// void Calculate_Membership(float value, float membership[FUZZY_SET_NUM])
// {
//     uint8 i;
//     const float parameters[FUZZY_SET_NUM][4] = {
//         {-100, -100, -75, -50}, // NB
//         {-75, -50, -50, -25},   // NM
//         {-50, -25, -25, 0},     // NS
//         {-25, 0, 0, 25},        // ZO
//         {0, 25, 25, 50},        // PS
//         {25, 50, 50, 75},       // PM
//         {50, 75, 100, 100}      // PB
//     };

//     // 清空隶属度数组
//     for (i = 0; i < FUZZY_SET_NUM; i++)
//     {
//         membership[i] = 0.0;
//     }

//     // 确定需要计算的隶属度区间
//     for (i = 0; i < FUZZY_SET_NUM; i++)
//     {
//         if (value >= parameters[i][0] && value <= parameters[i][3])
//         {
//             membership[i] = Membership_Function(value, parameters[i][0], parameters[i][1], parameters[i][2], parameters[i][3]);
//         }
//     }
// }

// // 模糊推理和去模糊化（最大隶属度法）
// float Fuzzy_Inference(float e_membership[FUZZY_SET_NUM], float ec_membership[FUZZY_SET_NUM])
// {
//     uint8 i;
//     float max_membership = 0.0;
//     float output = 0.0;

//     for (i = 0; i < FUZZY_SET_NUM; i++)
//     {
//         for (int j = 0; j < FUZZY_SET_NUM; j++)
//         {
//             float membership = e_membership[i] * ec_membership[j];
//             if (membership > max_membership)
//             {
//                 max_membership = membership;
//                 output = fuzzy_rule_table[i][j];
//             }
//         }
//     }
//     return output;
// }
// /* 隶属度法解模糊 END*/
