#include "headfile.h"

#define ENCODER_DIR_1 P35 // 左轮编码器方向脚，电机轴针对自己，值为1代表逆时针转(向前正转)
#define ENCODER_DIR_2 P53 // 右轮编码器方向脚

EncoderTypeDef encoder_1, encoder_2;

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
    Encoder_Parameter_Init(&encoder_1);
    Encoder_Parameter_Init(&encoder_2);
}

void Read_Encoder(void)
{
    if (ENCODER_DIR_1 == 1) // 输出高电平，正转
    {
        encoder_1.encoder_now = ctimer_count_read(CTIM0_P34);
    }
    else // 输出低电平，反转
    {
        encoder_1.encoder_now = abs(ctimer_count_read(CTIM0_P34));
    }

    if (ENCODER_DIR_2 == 1) // 右轮
    {
        encoder_2.encoder_now = abs(ctimer_count_read(CTIM3_P04));
    }
    else
    {
        encoder_2.encoder_now = ctimer_count_read(CTIM3_P04);
    }

    ctimer_count_clean(CTIM0_P34);
    ctimer_count_clean(CTIM3_P04); // 清除积累
}