#include "encoder.h"

EncoderTypeDef encoder_left, encoder_right;
float car_distance_cm = 0;

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

void Encoder_Process(void)
{
    Read_Encoder();
    Encoder_Filter();
}

void Read_Encoder(void)
{
    encoder_left.encoder_last = encoder_left.encoder_now;
    encoder_right.encoder_last = encoder_right.encoder_now;

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

#define ENCODER_FILTER_COEFFICIENT 0.9
void Encoder_Filter(void)
{
    encoder_left.encoder_filtered = Low_Pass_Filter(encoder_left.encoder_now, encoder_left.encoder_last, ENCODER_FILTER_COEFFICIENT);
    encoder_right.encoder_filtered = Low_Pass_Filter(encoder_right.encoder_now, encoder_right.encoder_last, ENCODER_FILTER_COEFFICIENT);
}

/*
 * @name: void Speed_Calcu(void);
 * @function: ����С������ʵ�ʾ���
 * @param: none
 * @return: none
 */
#define ENCODER_TO_DISTANCE_CM 0.0091 // ϵ��=�̶�����/���Եõ������� ����200cm ����21533 22350 22344
void Distance_Calculation(void)
{
    float car_distance_once = 0.0;

    car_distance_once = (encoder_left.encoder_filtered + encoder_right.encoder_filtered) / 2.0;
    car_distance_cm += ENCODER_TO_DISTANCE_CM * car_distance_once; // ʵ�ʾ���=����*ϵ��
}

void Distance_Reset(void)
{
    car_distance_cm = 0.0;
}