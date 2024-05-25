#include "encoder.h"

EncoderTypeDef encoder_left, encoder_right;
int32 car_distance = 0;
float real_distance = 0;

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

void Encoder_Filter(void)
{
    encoder_left.encoder_filtered = Low_Pass_Filter(encoder_left.encoder_now, encoder_left.encoder_last, 0.5);
    encoder_right.encoder_filtered = Low_Pass_Filter(encoder_right.encoder_now, encoder_right.encoder_last, 0.5);
}

/*
 * @name: void Speed_Calcu(void);
 * @function: 计算小车运行实际距离
 * @param: none
 * @return: none
 */
#define ENCODER_TO_DISTANCE 0.0091 // 系数=固定距离/测试得到的脉冲 测试200cm 脉冲21533 22350 22344
void Distance_Calculation(void)
{
    car_distance += (encoder_left.encoder_filtered + encoder_right.encoder_filtered) / 2;
    
    real_distance = ENCODER_TO_DISTANCE * car_distance; // 实际距离=脉冲*系数
}