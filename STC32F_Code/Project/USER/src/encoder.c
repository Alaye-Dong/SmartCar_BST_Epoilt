#include "headfile.h"

EncoderTypeDef encoder_left, encoder_right;
float car_distance = 0;
float real_distance = 0;

// ��������ʼ��
void Encoder_Init(void)
{
    // ��������ʼ��
    ctimer_count_init(CTIM0_P34);
    ctimer_count_init(CTIM3_P04);

    EncoderType_Init();
}

void Encoder_Parameter_Init(EncoderTypeDef *sptr)
{
    sptr->encoder_now = 0;
    sptr->encoder_last = 0;
}

void EncoderType_Init(void)
{
    Encoder_Parameter_Init(&encoder_left);
    Encoder_Parameter_Init(&encoder_right);
}

void Read_Encoder(void)
{
    if (ENCODER_DIR_1 == 1) // ����ߵ�ƽ����ת
    {
        encoder_left.encoder_now = ctimer_count_read(CTIM0_P34);
    }
    else // ����͵�ƽ����ת
    {
        encoder_left.encoder_now = ctimer_count_read(CTIM0_P34) * -1;
    }

    if (ENCODER_DIR_2 == 1) // ����
    {
        encoder_right.encoder_now = ctimer_count_read(CTIM3_P04) * -1;
    }
    else
    {
        encoder_right.encoder_now = ctimer_count_read(CTIM3_P04);
    }

    ctimer_count_clean(CTIM0_P34);
    ctimer_count_clean(CTIM3_P04); // �������
}

/*
 * @name: void Speed_Calcu(void);
 * @function: ����С������ʵ�ʾ���
 * @param: none
 * @return: none
 */
#define ENCODER_TO_DISTANCE 0.001 // ������ϵ��=�̶�����/���Եõ������壩
void Distance_Calculation(void)
{
    car_distance += (encoder_left.encoder_now + encoder_right.encoder_now) / 2;
    // ʵ�ʾ���=����*ϵ��
    real_distance = ENCODER_TO_DISTANCE * car_distance;
}

// /*
//  * @name:int16 Encoder_SLimit(int16 SpeedNew, int16 SpeedOld, int16 Grad)
//  * @function:�������ٶȲɼ��ݶ�ƽ����ÿ�βɼ����仯Grad
//  * @param:input
//  * @return:none
//  */
// int16 Encoder_SLimit(int16 SpeedNew, int16 SpeedOld, int16 Grad)
// {
//     int16 SpeedFinal = 0;

//     if (ABS(SpeedNew - SpeedOld) > Grad) {
//         if (SpeedNew > 0) {
//             SpeedFinal = SpeedOld + Grad;
//         } else if (SpeedNew < 0) {
//             SpeedFinal = SpeedOld - Grad;
//         }
//     } else {
//         SpeedFinal = SpeedNew;
//     }

//     return SpeedFinal;
// }