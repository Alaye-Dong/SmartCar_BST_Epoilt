#include "encoder.h"

EncoderTypeDef encoder_left, encoder_right;
float car_distance_cm = 0;

// 编码器初始化
void Encoder_Init(void)
{
    // 计数器初始化
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

    if (ENCODER_DIR_1 == 1) // 输出高电平，正转
    {
        encoder_left.encoder_now = ctimer_count_read(CTIM0_P34);
    }
    else // 输出低电平，反转
    {
        encoder_left.encoder_now = ctimer_count_read(CTIM0_P34) * -1;
    }

    if (ENCODER_DIR_2 == 1) // 右轮
    {
        encoder_right.encoder_now = ctimer_count_read(CTIM3_P04) * -1;
    }
    else
    {
        encoder_right.encoder_now = ctimer_count_read(CTIM3_P04);
    }

    ctimer_count_clean(CTIM0_P34);
    ctimer_count_clean(CTIM3_P04); // 清除积累
}

#define ENCODER_FILTER_COEFFICIENT 0.9
void Encoder_Filter(void)
{
    encoder_left.encoder_filtered = Low_Pass_Filter(encoder_left.encoder_now, encoder_left.encoder_last, ENCODER_FILTER_COEFFICIENT);
    encoder_right.encoder_filtered = Low_Pass_Filter(encoder_right.encoder_now, encoder_right.encoder_last, ENCODER_FILTER_COEFFICIENT);
}

/*
 * @name: void Speed_Calcu(void);
 * @function: 计算小车运行实际距离
 * @param: none
 * @return: none
 */
#define ENCODER_TO_DISTANCE_CM 0.0091 // 系数=固定距离/测试得到的脉冲 测试200cm 脉冲21533 22350 22344
void Distance_Calculation(void)
{
    float car_distance_once = 0.0;

    car_distance_once = (encoder_left.encoder_filtered + encoder_right.encoder_filtered) / 2.0;
    car_distance_cm += ENCODER_TO_DISTANCE_CM * car_distance_once; // 实际距离=脉冲*系数
}

void Distance_Reset(void)
{
    car_distance_cm = 0.0;
}