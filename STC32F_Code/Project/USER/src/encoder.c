#include "headfile.h"

#define ENCODER_DIR_1 P35 // ���ֱ���������ţ����������Լ���ֵΪ1������ʱ��ת(��ǰ��ת)
#define ENCODER_DIR_2 P53 // ���ֱ����������

EncoderTypeDef encoder_1, encoder_2;

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
    Encoder_Parameter_Init(&encoder_1);
    Encoder_Parameter_Init(&encoder_2);
}

void Read_Encoder(void)
{
    if (ENCODER_DIR_1 == 1) // ����ߵ�ƽ����ת
    {
        encoder_1.encoder_now = ctimer_count_read(CTIM0_P34);
    }
    else // ����͵�ƽ����ת
    {
        encoder_1.encoder_now = abs(ctimer_count_read(CTIM0_P34));
    }

    if (ENCODER_DIR_2 == 1) // ����
    {
        encoder_2.encoder_now = abs(ctimer_count_read(CTIM3_P04));
    }
    else
    {
        encoder_2.encoder_now = ctimer_count_read(CTIM3_P04);
    }

    ctimer_count_clean(CTIM0_P34);
    ctimer_count_clean(CTIM3_P04); // �������
}