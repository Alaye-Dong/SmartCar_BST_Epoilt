#include "pid.h"

PIDTypeDef direction, motor_left, motor_right;

int16 position_delta_error = 0; // ���ڷ���ģ��PID��ƫ��仯��
float direction_output = 0;
int16 position_last = 0;

int16 motor_left_error = 0, motor_right_error = 0;
int16 motor_left_last_error = 0, motor_right_last_error = 0;

uint8 direction_pid_time_counter = 0; // ���򻷿������ڱ�־λ ��20ms

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

// TODO ���ں��ʵ�ǰ��ϵ��
void PIDType_Init(void)
{
    PID_Parameter_Init(&direction, 0, 0, 0, 0, 0, 0);
    // PID_Parameter_Init(&direction, 0.45, 0, 2, 0.001, 0.006, 0.1); // 70�� // * ���ʹ�÷���ģ��PID���˴��������ý���Ч
    PID_Parameter_Init(&motor_left, 27.33, 2.737, 0, 0, 0, 0.0); // һ��̶��ٶ�P
    PID_Parameter_Init(&motor_right, 30.28, 3.818, 0, 0, 0, 0.0);
    // PID_Parameter_Init(&motor_left, 27.33, 5, 0, 0, 0, 0.0); // һ��̶��ٶ�P
    // PID_Parameter_Init(&motor_right, 30.28, 5, 0, 0, 0, 0.0);

    // // *ʹ�ö�̬P
    // PID_Parameter_Init(&direction, 0.47, 0.0, 1.2, 0.001, 0.003, 0); // 50��
    // PID_Parameter_Init(&motor_left, 0, 2.737, 0.0, 0.0, 0.0, 0); // ��ʼΪ0��ʹ�ö�̬P
    // PID_Parameter_Init(&motor_right, 0, 3.818, 0.0, 0.0, 0.0, 0);
}

void PID_Init(void)
{
    PIDType_Init();
}

#define DIRECTION_PID_PERIOD 4 // ����20ms�����ڣ���4��5ms����
void PID_Process(void)
{
    Speed_Contrl();
    // Position_Loss_Remember();

    if (direction_pid_time_counter != DIRECTION_PID_PERIOD - 1) // ���򻷿�������Ϊ20ms����3��5ms�жϱ�־λ������һ���ж�ʱ��20ms
    {
        direction_pid_time_counter++;
    }
    else
    {
        direction_pid_time_counter = 0; // �������ڼ�����

        Direction_PID();
    }

    Left_Speed_PID();
    Right_Speed_PID();
}

//  ���߼�����
uint8 position_loss_flag = 0;
void Position_Loss_Remember(void)
{
    static uint8 position_loss_time_counter = 0;

    if (position_loss_time_counter > 200) // 5 * 200 = 1000ms �����ۼ�1sͣ������
    {
        speed.target = 0;
    }

    if ((inductor[LEFT_H] <= 5 && inductor[RIGHT_H] <= 5)) // ��ʱ�䶪�ߣ�������
    {
        position = position_last;
        if (position_loss_time_counter < 255) // ��ֹ���
        {
            position_loss_time_counter++;
        }
    }
    else // Ѱ���ߣ������ۼ�ʱ���־λ����
    {
        position_loss_time_counter = 0;
    }
}

// *******************************����PID ƫ��->>ת��->>�ٶȻ�->>PWM
//  ת���⻷ PD ������ PD
void Direction_PID(void)
{
    position_delta_error = position - position_last;                                                                   // �������仯��
    Fuzzy_PID_Control(position, position_delta_error, &direction.KP, &direction.KD, &direction.KP_2, &direction.KD_2); // ģ��PID���㸳ֵ

    // ��������ת���Ѹ�٣�ֱ�������Ƚ��͡��ں������ǣ�ת���������ᣬ��ƽ�ȡ�
    direction_output = position * direction.KP + (position - position_last) * direction.KD + abs(position) * position * direction.KP_2 - gyro_z_filtered * direction.KD_2;
    // direction_output += position_delta_error * direction.KF; // �ϲ�ǰ����
    position_last = position;

    speed.target_left = speed.target - direction_output;
    speed.target_right = speed.target + direction_output;
}

// �����ڻ� PI
void Left_Speed_PID(void)
{
    motor_left_error = (int16)(speed.target_left - encoder_left.encoder_filtered);
    // motor_left.KP = Increment_PI_Dynamic_P_MAX(speed.target_left, encoder_left.encoder_filtered); // ʹ������ʽ��̬P
    motor_left_pwm += (motor_left_error - motor_left_last_error) * motor_left.KP + motor_left_error * motor_left.KI;
    motor_left_pwm += motor_left_error * motor_left.KF; // �ϲ�ǰ����

    motor_left_last_error = motor_left_error;
}

// �����ڻ� PI
void Right_Speed_PID(void)
{
    motor_right_error = (int16)(speed.target_right - encoder_right.encoder_filtered);
    // motor_right.KP = Increment_PI_Dynamic_P_MAX(speed.target_right, encoder_right.encoder_filtered); // ʹ������ʽ��̬P
    motor_right_pwm += (motor_right_error - motor_right_last_error) * motor_right.KP + motor_right_error * motor_right.KI;
    motor_right_pwm += motor_right_error * motor_right.KF; // �ϲ�ǰ����

    motor_right_last_error = motor_right_error;
}

/* �ٶȿ��ƣ�������� */
uint16 straight_time_flag = 0;
void Speed_Contrl(void)
{
    // TODO ���������ǽ���ֱ������ж�
    speed.target = speed.normal - FUNC_ABS(gyro_z_filtered * speed.deceleration_factor); // �ٶȿ��ƣ��������

    // //��ֱ������
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
 * ��̬��������PI���Ƶ�P�����ĺ���,�������PΪ���������ż������������ֵ
 * ���������ڸ��ݱ���������ʵ�ֶ�P�����Ķ�̬����
 *
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

/* ����ģ���Ӽ� */
#define NB 0 // Negative Big
#define NM 1 // Negative Medium
#define NS 2 // Negative Small
#define ZO 3 // Zero
#define PS 4 // Positive Small
#define PM 5 // Positive Medium
#define PB 6 // Positive Big

// ģ������������ƫ���ƫ��仯��ת��Ϊģ������
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

// ģ�������
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

// ʹ��ģ��������ģ��
uint8 Fuzzy_Rule(int error, int delta_error)
{
    // ȷ���������ͺͱ仯��������Ч��Χ��
    if (error < NB || error > PB || delta_error < NB || delta_error > PB)
    {
        return ZO; // Ĭ�Ϸ���ֵ��ΪZ�����쳣�����
    }

    return fuzzy_rule_table[error][delta_error];
}

// ģ�� PID ������
void Fuzzy_PID_Control(float error, float delta_error, float *kp, float *kd, float *kp2, float *kd2)
{
    int16 fuzzy_error = Fuzzify(error);             // ģ�������
    int16 fuzzy_delta_error = Fuzzify(delta_error); // ģ�������仯��

    uint8 fuzzy_output = Fuzzy_Rule(fuzzy_error, fuzzy_delta_error);

    if (speed.normal <= 70)
    {
        // ����ģ��������� PID ����
        switch (fuzzy_output)
        {
        case NB:
        case PB: // ��Ϊ������ת������ȫ�Գƣ�����������Խ�����ģ�������ͬһ��ֵ
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
        // ����ģ��������� PID ����
        switch (fuzzy_output)
        {
        case NB:
        case PB: // ��Ϊ������ת������ȫ�Գƣ�����������Խ�����ģ�������ͬһ��ֵ
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
