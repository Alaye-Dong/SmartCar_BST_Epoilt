#include "pid.h"

PIDTypeDef direction, motor_left, motor_right;

float direction_output = 0;
int16 position_last = 0;

int16 motor_left_error = 0, motor_right_error = 0;
int16 motor_left_last_error = 0, motor_right_last_error = 0;

uint8 direction_pid_time_flag = 0; // ���򻷿������ڱ�־λ ��20ms

SpeedTypeDef speed = {50, 0, 0.04,
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
    // PID_Parameter_Init(&direction, 0.45, 0.0, 1.2, 0.001, 0.002); // 50��
    //  70�� PID_Parameter_Init(&direction, 0.5, 0.0, 1.2, 0.001, 0.006);

    // PID_Parameter_Init(&motor_left, 27.33, 2.737, 0.0, 0.0, 0.0); //һ��̶��ٶ�P
    // PID_Parameter_Init(&motor_right, 30.28, 3.818, 0.0, 0.0, 0.0);

    // *ʹ�ö�̬P
    PID_Parameter_Init(&direction, 0.45, 0.0, 1.2, 0.001, 0.004); // 50��
    PID_Parameter_Init(&motor_left, 0, 2.737, 0.0, 0.0, 0.0);     // ��ʼΪ0��ʹ�ö�̬P
    PID_Parameter_Init(&motor_right, 0, 3.818, 0.0, 0.0, 0.0);
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

    speed.target_left = speed.target - direction_output;
    speed.target_right = speed.target + direction_output;
}

// �����⻷ PI
void Left_Speed_PID(void)
{
    motor_left_error = (int16)(speed.target_left - encoder_left.encoder_filtered);
    motor_left.KP = Increment_PI_Dynamic_P_MAX(speed.target_left, encoder_left.encoder_now);
    motor_left_pwm += (motor_left_error - motor_left_last_error) * motor_left.KP + motor_left_error * motor_left.KI;
    motor_left_last_error = motor_left_error;
}

// �����⻷ PI
void Right_Speed_PID(void)
{
    motor_right_error = (int16)(speed.target_right - encoder_right.encoder_filtered);
    motor_right.KP = Increment_PI_Dynamic_P_MAX(speed.target_right, encoder_right.encoder_now);
    motor_right_pwm += (motor_right_error - motor_right_last_error) * motor_right.KP + motor_right_error * motor_right.KI;
    motor_right_last_error = motor_right_error;
}

// �ٶȿ��ƣ��������
void Speed_Contrl(void)
{
    // speed.target = speed.normal;
    speed.target = speed.normal - FUNC_ABS(position * speed.deceleration_factor);

    //////////**************ֱ��***********/////////////
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
//     motor_left_error = (int16)(speed.target - encoder_left.encoder_now);
//     motor_left_pwm += (motor_left_error - motor_left_last_error) * motor_left.KP + motor_left_error * motor_left.KI;
//     motor_left_last_error = motor_left_error;

//     motor_left_pwm -= direction_output; // �ںϷ������
// }

// // �����ڻ� PI
// void Right_Speed_PID(void)
// {
//     motor_right_error = (int16)(speed.target - encoder_right.encoder_now);
//     motor_right_pwm += (motor_right_error - motor_right_last_error) * motor_right.KP + motor_right_error * motor_right.KI;
//     motor_right_last_error = motor_right_error;

//     motor_right_pwm += direction_output; // �ںϷ������
// }

/**
 * ��̬��������PI���Ƶ�P�����ĺ���,�������PΪ���������ż������������ֵ
 * ���������ڸ��ݱ���������ʵ�ֶ�P�����Ķ�̬����
 *
 * @param pwm_last ��һ���ڵ�PWMռ�ձȣ�int16���ͣ�
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
