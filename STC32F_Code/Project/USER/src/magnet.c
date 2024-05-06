#include "magnet.h"

/**********全局变量定义********/

float g_fDirectionError[5];     // 方向偏差（g_fDirectionError[0]为一对水平电感的差比和偏差）
float g_fDirectionError_dot[3]; // 方向偏差微分（g_fDirectionError_dot[0]为一对水平电感的差比和偏差微分）

int16 ADC_single_divided_value[6] = {0}; // 获取的电感值
int16 ADC_filtered_value[5] = {0};

// 定义数组大小常量，提高可读性和可维护性
#define INDUCTOR 6 // 电感的个数
#define SAMPLES 5  // 单次采集次数
int16 ADC_value[INDUCTOR][SAMPLES];

void Magnet_ADC_Init(void)
{
    adc_init(ADC_P00, ADC_SYSclk_DIV_2);
    adc_init(ADC_P01, ADC_SYSclk_DIV_2);
    adc_init(ADC_P05, ADC_SYSclk_DIV_2);
    adc_init(ADC_P06, ADC_SYSclk_DIV_2);
    adc_init(ADC_P11, ADC_SYSclk_DIV_2);
    adc_init(ADC_P13, ADC_SYSclk_DIV_2);
}

void Magnet_ADC_Read(void)
{
    uint8 i;
    // [左横 3] [左斜 4] [左竖 0] || [右竖 5] [右斜 1] [右横 2]
    for (i = 0; i < SAMPLES; i++)
    {
        ADC_value[0][i] = adc_once(ADC_P11, ADC_12BIT); // zui you
        ADC_value[1][i] = adc_once(ADC_P05, ADC_12BIT); // zhong jian
        ADC_value[2][i] = adc_once(ADC_P00, ADC_12BIT); // 横
        ADC_value[3][i] = adc_once(ADC_P13, ADC_12BIT); // 横
        ADC_value[4][i] = adc_once(ADC_P06, ADC_12BIT); // zui zuo
        ADC_value[5][i] = adc_once(ADC_P01, ADC_12BIT); // 05     /
    }
}

void Magnet_ADC_Filter(void)
{
    int16 i, j;
    int16 ADC_median_value[INDUCTOR], ADC_sum[INDUCTOR];
    int16 ADC_old_filtered_value[INDUCTOR], ADC_new_filtered_value[INDUCTOR];

    Magnet_ADC_Read();

    /*=========================冒泡排序升序==========================*/ // 舍弃最大值和最小值
    // 调用排序函数，对每一行进行排序
    Bubble_Sort_ADC();

    /*===========================中值滤波=================================*/
    for (i = 0; i < INDUCTOR; i++) 
    {
        for(j = 1; j < SAMPLES - 1; j++) // 求去除最大和最小项的和
        {
            ADC_sum[i] += ADC_value[i][j];
        }
        
        ADC_median_value[i] = ADC_sum[i] / (SAMPLES - 2);
    }

    for (i = 0; i < INDUCTOR; i++) // 将数值中个位数除掉
    {
        ADC_single_divided_value[i] = (int16)(ADC_median_value[i] / 10 * 10);

        // 采集梯度平滑，每次采集最大变化40
        ADC_old_filtered_value[i] = ADC_filtered_value[i];
        ADC_new_filtered_value[i] = ADC_single_divided_value[i];

        if (ADC_new_filtered_value[i] >= ADC_old_filtered_value[i])
            ADC_filtered_value[i] = ((ADC_new_filtered_value[i] - ADC_old_filtered_value[i]) > 50 ? (ADC_old_filtered_value[i] + 50) : ADC_new_filtered_value[i]);
        else
            ADC_filtered_value[i] = ((ADC_new_filtered_value[i] - ADC_old_filtered_value[i]) < -60 ? (ADC_old_filtered_value[i] - 60) : ADC_new_filtered_value[i]);
    }
}

void Bubble_Sort_ADC(void)
{
    int i, j, k;
    uint8 swapped;
    int16 temp;

    for (i = 0; i < INDUCTOR; i++)
    {
        swapped = 0; // 每轮排序开始前，标记未发生交换

        for (j = 0; j < SAMPLES - 1; j++)
        {
            for (k = 0; k < SAMPLES - 1 - j; k++)
            {
                if (ADC_value[i][k] > ADC_value[i][k + 1])
                {
                    temp = ADC_value[i][k];
                    ADC_value[i][k] = ADC_value[i][k + 1];
                    ADC_value[i][k + 1] = temp;
                    swapped = 1; // 发生了交换，更新标记
                }
            }
            if (!swapped)
            { // 如果一轮循环没有发生交换，说明数组已排序
                break;
            }
        }
    }
}