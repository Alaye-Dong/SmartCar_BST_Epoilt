#include "headfile.h"

#define ENCODER_DIR_1 P35 // 左轮编码器方向脚，电机轴针对自己，值为1代表逆时针转(向前正转)
#define ENCODER_DIR_2 P53 // 右轮编码器方向脚

EncoderTypeDef encoder_left, encoder_right;
float car_distance = 0;
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

void Read_Encoder(void)
{
    if (ENCODER_DIR_1 == 1) // 输出高电平，正转
    {
        encoder_left.encoder_now = ctimer_count_read(CTIM0_P34);
    }
    else // 输出低电平，反转
    {
        encoder_left.encoder_now = abs(ctimer_count_read(CTIM0_P34));
    }

    if (ENCODER_DIR_2 == 1) // 右轮
    {
        encoder_right.encoder_now = abs(ctimer_count_read(CTIM3_P04));
    }
    else
    {
        encoder_right.encoder_now = ctimer_count_read(CTIM3_P04);
    }

    ctimer_count_clean(CTIM0_P34);
    ctimer_count_clean(CTIM3_P04); // 清除积累
}

/*
 * @name: void Speed_Calcu(void);
 * @function: 计算小车运行实际距离
 * @param: none
 * @return: none
 */
#define ENCODER_TO_DISTANCE 0.001 // 待定（系数=固定距离/测试得到的脉冲）
void Distance_Calculation(void)
{
    car_distance += (encoder_left.encoder_now + encoder_right.encoder_now) / 2;
    // 实际距离=脉冲*系数
    real_distance = ENCODER_TO_DISTANCE * car_distance;
}
