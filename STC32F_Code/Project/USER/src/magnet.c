#include "magnet.h"

int16 ADC_value[INDUCTORS][SAMPLES] = {0};
int16 ADC_filtered_value[INDUCTORS] = {0};
int16 inductor_normal_value[INDUCTORS] = {0};
int16 ADC_old_filtered_value[INDUCTORS] = {0};
int16 inductor[INDUCTORS] = {0};
int16 position_left = 0, position_right = 0, position = 0; // position大于0表示车偏右应左转，小于0表示车偏左应右转

void Magnet_ADC_Init(void)
{
    adc_init(ADC_P06, ADC_SYSclk_DIV_2);
    adc_init(ADC_P11, ADC_SYSclk_DIV_2);
    adc_init(ADC_P14, ADC_SYSclk_DIV_2);
    adc_init(ADC_P15, ADC_SYSclk_DIV_2);

    // 斜向待添加
}

void Inductor_Process(void)
{
    Magnet_ADC_Read();
    Magnet_ADC_Filter();
    Inductor_Normal();
}

void Magnet_ADC_Read(void)
{
    uint8 i;
    // 牛爷爷的车队 [左横 3] [左斜 4] [左竖 0] || [右竖 5] [右斜 1] [右横 2]
    for (i = 0; i < SAMPLES; i++)
    {
        ADC_value[LEFT_H][i] = adc_once(ADC_P06, ADC_12BIT);
        ADC_value[LEFT_V][i] = adc_once(ADC_P11, ADC_12BIT);
        ADC_value[LEFT_S][i] = 0; // 暂时不考虑斜向

        ADC_value[RIGHT_S][i] = 0; // 暂时不考虑斜向
        ADC_value[RIGHT_V][i] = adc_once(ADC_P14, ADC_12BIT);
        ADC_value[RIGHT_H][i] = adc_once(ADC_P15, ADC_12BIT);
    }
}

void Magnet_ADC_Filter(void)
{
    uint8 i;
    int16 ADC_median_value[INDUCTORS];

    // 冒泡排序
    Bubble_Sort_ADC();

    for (i = 0; i < INDUCTORS; i++)
    {
        ADC_old_filtered_value[i] = ADC_filtered_value[i];

        Trimmed_Mean_Filter(&ADC_value[i], INDUCTORS, EXTREME_NUMBER, &ADC_median_value[i]);

        ADC_filtered_value[i] = (int16)(ADC_median_value[i] / 10 * 10); // 将数值中个位数除掉

        // 梯度平滑
        if (ADC_filtered_value[i] - ADC_old_filtered_value[i] > 50)
        {
            ADC_filtered_value[i] = ADC_old_filtered_value[i] + 50;
        }
        else if (ADC_filtered_value[i] - ADC_old_filtered_value[i] < -60)
        {
            ADC_filtered_value[i] = ADC_old_filtered_value[i] - 60;
        }
    }
}

void Bubble_Sort_ADC(void)
{
    uint8 k;
    for (k = 0; k < INDUCTORS; k++)
    {
        Bubble_Sort_Int16(ADC_value[k], SAMPLES);
    }
}

// 电感归一化
void Inductor_Normal(void)
{
    // (x - min) / (max - min) * 100
    inductor_normal_value[LEFT_V] = (float)(ADC_filtered_value[LEFT_V] - 10.0) / (3700.0 - 10.0) * 100.0;
    inductor_normal_value[RIGHT_V] = (float)(ADC_filtered_value[RIGHT_V] - 10.0) / (3700.0 - 10.0) * 100.0;

    inductor_normal_value[LEFT_H] = (float)(ADC_filtered_value[LEFT_H] - 10.0) / (2000.0 - 10.0) * 100.0;
    inductor_normal_value[RIGHT_H] = (float)(ADC_filtered_value[RIGHT_H] - 10.0) / (2000.0 - 10.0) * 100.0;

    inductor_normal_value[LEFT_S] = (float)(ADC_filtered_value[LEFT_S] - 10.0) / (2280.0 - 10.0) * 100.0;
    inductor_normal_value[RIGHT_S] = (float)(ADC_filtered_value[RIGHT_S] - 10.0) / (2280.0 - 10.0) * 100.0;

    inductor[LEFT_V] = FUNC_LIMIT_AB(inductor_normal_value[LEFT_V], 0, 100);
    inductor[RIGHT_V] = FUNC_LIMIT_AB(inductor_normal_value[RIGHT_V], 0, 100);

    inductor[LEFT_H] = FUNC_LIMIT_AB(inductor_normal_value[LEFT_H], 0, 100);
    inductor[RIGHT_H] = FUNC_LIMIT_AB(inductor_normal_value[RIGHT_H], 0, 100);

    inductor[LEFT_S] = FUNC_LIMIT_AB(inductor_normal_value[LEFT_S], 0, 100);
    inductor[RIGHT_S] = FUNC_LIMIT_AB(inductor_normal_value[RIGHT_S], 0, 100);
}

void Position_Analyse(void)
{
    // 使用快速平方根的算法
    // position_left = My_Sqrt(inductor[LEFT_H] * inductor[LEFT_H] + inductor[LEFT_V] * inductor[LEFT_V]);
    // position_right = My_Sqrt(inductor[RIGHT_H] * inductor[RIGHT_H] + inductor[RIGHT_V] * inductor[RIGHT_V]);

    // 使用系统平方根的算法
    position_left = sqrt(inductor[LEFT_H] * inductor[LEFT_H] + inductor[LEFT_V] * inductor[LEFT_V]);
    position_right = sqrt(inductor[RIGHT_H] * inductor[RIGHT_H] + inductor[RIGHT_V] * inductor[RIGHT_V]);

    position = (position_left - position_right) * 100 / (position_left + position_right + 1); // 向量差比和，补1防止分母为0
}

void Magnet_ADC_Print(void)
{
    // uint8 i;
    // for (i = 0; i < INDUCTORS; i++)
    // {
    printf("%d,%d,%d\n", ADC_filtered_value[LEFT_H], inductor_normal_value[LEFT_H], inductor[LEFT_H]);
    // }
}
