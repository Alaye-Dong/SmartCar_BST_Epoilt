#include "magnet.h"

int16 ADC_values[INDUCTORS][SAMPLES] = {0};
int16 ADC_filtered_value[INDUCTORS] = {0};
int16 inductor[INDUCTORS] = {0};

int16 position_vector_modulus = 0;
int16 position_difference_weighted = 0;
int16 position_normal = 0;
int16 position = 0; // position大于0表示车偏右应左转，小于0表示车偏左应右转

void Magnet_ADC_Init(void)
{
    /* ADC引脚初始化 */
    adc_init(ADC_P06, ADC_SYSclk_DIV_2);
    adc_init(ADC_P11, ADC_SYSclk_DIV_2);
    adc_init(ADC_P14, ADC_SYSclk_DIV_2);
    adc_init(ADC_P15, ADC_SYSclk_DIV_2);
    // 斜向待添加

    // /* 电感获取自检 */
    // Inductor_Process();

    // if (inductor[LEFT_H] == 0 && inductor[RIGHT_H] == 0 && inductor[LEFT_V] == 0 && inductor[RIGHT_V] == 0)
    // {
    //     lcd_clear(YELLOW);
    //     lcd_showstr(1 * 8, 2, "MAGNET_WARNING");
    // }
    // else if (inductor[LEFT_H] == 0 || inductor[RIGHT_H] == 0 || inductor[LEFT_V] == 0 || inductor[RIGHT_V] == 0)
    // {
    //     lcd_clear(RED);
    //     lcd_showstr(1 * 8, 2, "MAGNET_ERROR");
    // }
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

    for (i = 0; i < SAMPLES; i++)
    {
        ADC_values[LEFT_H][i] = adc_once(ADC_P06, ADC_12BIT);
        ADC_values[LEFT_V][i] = adc_once(ADC_P11, ADC_12BIT);
        ADC_values[LEFT_S][i] = 0; // 暂时不使用斜向

        ADC_values[RIGHT_S][i] = 0; // 暂时不使用斜向
        ADC_values[RIGHT_V][i] = adc_once(ADC_P14, ADC_12BIT);
        ADC_values[RIGHT_H][i] = adc_once(ADC_P15, ADC_12BIT);
    }
}

void Magnet_ADC_Filter(void)
{
    uint8 inductor_index;
    int16 ADC_mean_value[INDUCTORS];
    static int16 ADC_old_filtered_value[INDUCTORS] = {0};

    // // 冒泡排序
    // Bubble_Sort_ADC();

    for (inductor_index = 0; inductor_index < INDUCTORS; inductor_index++)
    {
        ADC_old_filtered_value[inductor_index] = ADC_filtered_value[inductor_index];

        // Trimmed_Mean_Filter(&ADC_values[i], INDUCTORS, EXTREME_NUMBER, &ADC_mean_value[i]);
        Fast_De_Extremum_Averaging(&ADC_values[inductor_index], SAMPLES, &ADC_mean_value[inductor_index]);

        ADC_filtered_value[inductor_index] = (int16)(ADC_mean_value[inductor_index] / 10 * 10); // 将数值中个位数除掉

        // // 梯度平滑
        // if (ADC_filtered_value[inductor_index] - ADC_old_filtered_value[inductor_index] > 50)
        // {
        //     ADC_filtered_value[inductor_index] = ADC_old_filtered_value[inductor_index] + 50;
        // }
        // else if (ADC_filtered_value[inductor_index] - ADC_old_filtered_value[inductor_index] < -60)
        // {
        //     ADC_filtered_value[inductor_index] = ADC_old_filtered_value[inductor_index] - 60;
        // }
    }
}

/**
 * @description: 快速去除极值求平均滤波
 * @param {int16} input_array
 * @param {uint8} array_length
 * @param {int16} *output_avage
 * @return {*}
 * ! @note: 此方面相对于冒泡排序再舍弃极值求平均，省去了对于中间的数即非极值不必要的排序，计算更快速。
 * * 但是舍弃的极值个数只能为各一个最大和最小值，但是不能控制舍弃的极值个数。
 */
void Fast_De_Extremum_Averaging(int16 input_array[], uint8 array_length, int16 *output_avage)
{
    uint8 i;
    int16 extremum_max = 0;
    int16 extremum_min = 0;
    int16 sum = 0;

    for (i = 0; i < array_length; i++)
    {
        if (input_array[i] > extremum_max)
        {
            extremum_max = input_array[i];
        }
        if (input_array[i] < extremum_min)
        {
            extremum_min = input_array[i];
        }
        sum += input_array[i];
    }
    *output_avage = (sum - extremum_max - extremum_min) / (array_length - 2);
}

void Bubble_Sort_ADC(void)
{
    uint8 inductor_index;
    for (inductor_index = 0; inductor_index < INDUCTORS; inductor_index++)
    {
        Bubble_Sort_Int16(ADC_values[inductor_index], SAMPLES);
    }
}

// 电感归一化
#define LEFT_V_MAX 3000
#define LEFT_V_MIN 25
#define RIGHT_V_MAX 3000
#define RIGHT_V_MIN 1
#define LEFT_H_MAX 2300
#define LEFT_H_MIN 35
#define RIGHT_H_MAX 2300
#define RIGHT_H_MIN 15
void Inductor_Normal(void)
{
    static int16 inductor_normal_value[INDUCTORS] = {0};
    // 归一化公式 (x - min) / (max - min) * 100

    inductor_normal_value[LEFT_V] = (float)(ADC_filtered_value[LEFT_V] - LEFT_V_MIN) / (LEFT_V_MAX - LEFT_V_MIN) * 100;
    inductor_normal_value[RIGHT_V] = (float)(ADC_filtered_value[RIGHT_V] - RIGHT_V_MIN) / (RIGHT_V_MAX - RIGHT_V_MIN) * 100;
    inductor_normal_value[LEFT_H] = (float)(ADC_filtered_value[LEFT_H] - LEFT_H_MIN) / (LEFT_H_MAX - LEFT_H_MIN) * 100;
    inductor_normal_value[RIGHT_H] = (float)(ADC_filtered_value[RIGHT_H] - RIGHT_H_MIN) / (RIGHT_H_MAX - RIGHT_H_MIN) * 100;

    inductor[LEFT_V] = FUNC_LIMIT_AB(inductor_normal_value[LEFT_V], 0, 100);
    inductor[RIGHT_V] = FUNC_LIMIT_AB(inductor_normal_value[RIGHT_V], 0, 100);
    inductor[LEFT_H] = FUNC_LIMIT_AB(inductor_normal_value[LEFT_H], 0, 100);
    inductor[RIGHT_H] = FUNC_LIMIT_AB(inductor_normal_value[RIGHT_H], 0, 100);

    // inductor_normal_value[LEFT_S] = (float)(ADC_filtered_value[LEFT_S] - 10.0) / (2280.0 - 10.0) * 100.0;
    // inductor_normal_value[RIGHT_S] = (float)(ADC_filtered_value[RIGHT_S] - 10.0) / (2280.0 - 10.0) * 100.0;
    // inductor[LEFT_S] = FUNC_LIMIT_AB(inductor_normal_value[LEFT_S], 0, 100);
    // inductor[RIGHT_S] = FUNC_LIMIT_AB(inductor_normal_value[RIGHT_S], 0, 100);
}

#define V_GAIN 2 // 垂直方向的增益
#define H_GAIN 1

void Position_Analyse(void)
{
    int16 temp_left = 0;
    int16 temp_right = 0;

    int16 temp_difference = 0;
    int16 temp_sum_difference_weighted = 0;

    int16 temp_sum = 0;

    // ! 向量模差比和算法
    temp_left = My_Sqrt(inductor[LEFT_H] * inductor[LEFT_H] + inductor[LEFT_V] * inductor[LEFT_V]); // TODO 测试对竖电感加权加权
    temp_right = My_Sqrt(inductor[RIGHT_H] * inductor[RIGHT_H] + inductor[RIGHT_V] * inductor[RIGHT_V]);
    position_vector_modulus = (temp_left - temp_right) * 100 / (temp_left + temp_right + 1); // 向量差比和，补1防止分母为0 //TODO : 测试改成差加权会不会更好用

    // * 差比和差加权算法
    temp_difference = H_GAIN * (inductor[LEFT_H] - inductor[RIGHT_H]) + V_GAIN * (inductor[LEFT_V] - inductor[RIGHT_V]); // TODO 测试加权系数
    temp_sum_difference_weighted = H_GAIN * (inductor[LEFT_H] + inductor[RIGHT_H]) + abs((inductor[LEFT_V] - inductor[RIGHT_V]));
    position_difference_weighted = (temp_difference * 100) / (temp_sum_difference_weighted + 1);

    //  普通差比和
    temp_sum = (inductor[LEFT_H] + inductor[RIGHT_H]) + (inductor[LEFT_V] - inductor[RIGHT_V]);
    position_normal = (temp_difference * 100) / (temp_sum + 1);

    position = position_difference_weighted;
}
