#include "pid.h"

PIDTypeDef direction, motor_left, motor_right;

int16 position_delta_error = 0; // ���ڷ���ģ��PID��ƫ��仯��
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
    PID_Parameter_Init(&direction, 0.5, 0, 6.8, 0.0015, 0.002, 5.6); // * ���ʹ�÷���ģ��PID���˴���P��I��D�������ý���Ч
    PID_Parameter_Init(&motor_left, 27.33, 2.737, 0, 0, 0, 0.0);
    PID_Parameter_Init(&motor_right, 30.28, 3.818, 0, 0, 0, 0.0);
}

void PID_Init(void)
{
    PIDType_Init();
}

#define DIRECTION_PID_PERIOD 4 // ����20ms�����ڣ���4��5ms����
// PID���������У��⻷����Ӧ�ô��ڵ����ڻ����ڣ�ʹ���ڻ��������㹻��ʱ����Ӧ�⻷������
void PID_Process(void)
{
    static uint8 direction_pid_time_counter = 0; // ���򻷿������ڱ�־λ

    if (direction_pid_time_counter != DIRECTION_PID_PERIOD - 1) // ���򻷿�������Ϊ20ms����3��5ms�жϱ�־λ������һ���ж�ʱ��20ms
    {
        direction_pid_time_counter++;
    }
    else
    {
        direction_pid_time_counter = 0; // �������ڼ�����
        Speed_Contrl();
        Direction_PID();
        // Direction_PID_Incomplete_D();
    }

    Left_Speed_PID();
    Right_Speed_PID();
}

// TODO ǰ��PID
#define DELTA_DECELERATION_FACTOR 2 // TODO ������
void Speed_Contrl(void)
{
    // TODO ���������ǽ���ֱ������ж�
    speed.target = speed.normal -
                   FUNC_ABS(gyro_z_filtered * speed.deceleration_factor) -
                   FUNC_ABS(position_delta_error * DELTA_DECELERATION_FACTOR); // �ٶȿ��ƣ��������

    if (speed.target < 0)
    {
        speed.target = 0;
    }

    if (position_loss_stop_protect_flag == 1)
    {
        speed.target = 0; // ����ͣ������
    }

    // //��ֱ������
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

// *******************************����PID ƫ��->>ת��->>���ٶȻ�����ʵ�֣�->>�ٶȻ�->>PWM
//  ת���⻷ PD ������ PD
void Direction_PID(void)
{
    static int16 position_last = 0;

    position_delta_error = position - position_last;               // �������仯��
    Fuzzy_PID_Control(position, position_delta_error, &direction); // ģ��PID���㸳ֵ

    direction_output = position * direction.KP + (position - position_last) * direction.KD +
                       abs(position) * position * direction.KP_2 - gyro_z_filtered * direction.KD_2; // ��������ת���Ѹ�٣�ֱ�������Ƚ��͡��ں������ǣ�ת���������ᣬ��ƽ�ȡ�
    // direction_output += (position_delta_error / (0.005 * DIRECTION_PID_PERIOD) + position)* direction.KF; // �ϲ�ǰ����
    position_last = position;

    speed.target_left = speed.target - direction_output;
    speed.target_right = speed.target + direction_output;
}

/* TODO ʵ���� ����ȫ΢��PID */
#define TF T * 0.05
#define T 0.02
void Direction_PID_Incomplete_D(void)
{
    static int16 position_last = 0;
    static float D_value_last = 0;
    const float alpha = 0.05; // TF / (TF + T);
    float D_value = 0;

    position_delta_error = position - position_last;               // �������仯��
    Fuzzy_PID_Control(position, position_delta_error, &direction); // ģ��PID���㸳ֵ
    D_value = direction.KD * (1 - alpha) * position_delta_error + alpha * D_value_last;
    direction_output = position * direction.KP +
                       D_value +
                       abs(position) * position * direction.KP_2 - gyro_z_filtered * direction.KD_2; // ��������ת���Ѹ�٣�ֱ�������Ƚ��͡��ں������ǣ�ת���������ᣬ��ƽ�ȡ�

    // D_value = direction.KD * (1 - alpha) * position + alpha * D_value_last - direction.KD * (1 - alpha) * position_last;

    // ����Dֵ����ϵ�ǰλ�ò����һDֵ�ļ�Ȩ��

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

// �����ڻ� PI
void Left_Speed_PID(void)
{
    static int16 motor_left_last_error = 0;
    int16 motor_left_error = 0;

    motor_left_error = (int16)(speed.target_left - encoder_left.encoder_filtered);
    // motor_left.KP = Increment_PI_Dynamic_P_MAX(speed.target_left, encoder_left.encoder_filtered); // ʹ������ʽ��̬P
    motor_left_pwm += (motor_left_error - motor_left_last_error) * motor_left.KP + motor_left_error * motor_left.KI;

    motor_left_last_error = motor_left_error;
}

// �����ڻ� PI
void Right_Speed_PID(void)
{
    static int16 motor_right_last_error = 0;
    int16 motor_right_error = 0;

    motor_right_error = (int16)(speed.target_right - encoder_right.encoder_filtered);
    // motor_right.KP = Increment_PI_Dynamic_P_MAX(speed.target_right, encoder_right.encoder_filtered); // ʹ������ʽ��̬P
    motor_right_pwm += (motor_right_error - motor_right_last_error) * motor_right.KP + motor_right_error * motor_right.KI;

    motor_right_last_error = motor_right_error;
}

/**
 * ��̬��������PI���Ƶ�P�����ĺ���,�������PΪ���������ż������������ֵ
 * ���������ڸ��ݱ���������ʵ�ֶ�P�����Ķ�̬����
 * @note ʵ��Ч�������룬���о��Ż�
 * @param encoder_now ��һ���ڵ�ʵ���ٶȣ�int16���ͣ�
 * @param encoder_increment ���򣬼�������������int16���ͣ�������������ֵ��Ŀ��ֵ�Ĳencoder_target - encoder_now
 * @param ENCODER_MAX �޷�ֵ
 * @return ����P�����ĵ���ֵ��float���ͣ����������������Ϊ0���򷵻�һ��һ���Ե�Pֵ��30����
 */
#define ENCODER_MAX 100
float Increment_PI_Dynamic_P_MAX(int16 target_speed_encoder, int16 encoder_now)
{
    int16 encoder_increment = target_speed_encoder - encoder_now;
    if (encoder_increment == 0) // ��ĸ����Ϊ0
    {
        return 30; // ����һ��һ���Ե�P
    }

    return FUNC_ABS((float)(ENCODER_MAX - encoder_now) / encoder_increment);
}

// *********************************************ģ��PID

#define FUZZY_SET_NUM 7
/* ����ģ���Ӽ� */
#define NB 0 // Negative Big
#define NM 1 // Negative Medium
#define NS 2 // Negative Small
#define ZO 3 // Zero
#define PS 4 // Positive Small
#define PM 5 // Positive Medium
#define PB 6 // Positive Big

// ģ������
const int8 fuzzy_domain[FUZZY_SET_NUM] = {-75, -50, -25, 0, 25, 50, 75};

// ��������ȷ�������ֵƥ���
const int8 fuzzy_domain_maximum_membership_match[FUZZY_SET_NUM] = {(-75 + -50) / 2, (-50 + -25) / 2, (-25 + 0) / 2, (0 + 25) / 2, (25 + 50) / 2, (50 + 75) / 2, (50 + 75) / 2};

// ģ������������ƫ���ƫ��仯��ת��Ϊģ������
uint8 Fuzzify(float value)
{
    // ֱ��ӳ�䵽��ӽ���ģ���Ӽ�
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

    // �������㷨
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

// ģ�������
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

// ʹ��ģ��������ģ��
uint8 Fuzzy_Rule(int fuzzy_error, int fuzzy_delta_error)
{
    // ȷ���������ͺͱ仯��������Ч��Χ��
    if (fuzzy_error < NB || fuzzy_error > PB || fuzzy_delta_error < NB || fuzzy_delta_error > PB)
    {
        return ZO; // Ĭ�Ϸ���ֵ��ΪZ�����쳣�����
    }

    return fuzzy_rule_table[fuzzy_error][fuzzy_delta_error];
}

// ģ�� PID ������
void Fuzzy_PID_Control(float error, float delta_error, PIDTypeDef *sptr)
{
    int16 fuzzy_error = Fuzzify(error);             // ģ�������
    int16 fuzzy_delta_error = Fuzzify(delta_error); // ģ�������仯��

    uint8 fuzzy_output = Fuzzy_Rule(fuzzy_error, fuzzy_delta_error);

    // ����ģ��������� PID ����
    switch (fuzzy_output)
    {
    case NB:
    case PB: // ��Ϊ������ת������ȫ�Գƣ�����������Խ�����ģ�������ͬһ��ֵ
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
TODO �����ȷ���ģ��
*/
// // ���������Ⱥ���
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

// // �����������仯�ʵ�������
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

//     // �������������
//     for (i = 0; i < FUZZY_SET_NUM; i++)
//     {
//         membership[i] = 0.0;
//     }

//     // ȷ����Ҫ���������������
//     for (i = 0; i < FUZZY_SET_NUM; i++)
//     {
//         if (value >= parameters[i][0] && value <= parameters[i][3])
//         {
//             membership[i] = Membership_Function(value, parameters[i][0], parameters[i][1], parameters[i][2], parameters[i][3]);
//         }
//     }
// }

// // ģ�������ȥģ��������������ȷ���
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
// /* �����ȷ���ģ�� END*/
