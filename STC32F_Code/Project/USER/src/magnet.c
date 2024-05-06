#include "magnet.h"

#define INDUCTOR 6 // ��еĸ���
#define SAMPLES 5  // ���βɼ�����

int16 ADC_value[INDUCTOR][SAMPLES];
int16 ADC_single_divided_value[INDUCTOR] = {0}; // ��ȡ�ĵ��ֵ
int16 ADC_filtered_value[INDUCTOR] = {0};

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
    // ţүү�ĳ��� [��� 3] [��б 4] [���� 0] || [���� 5] [��б 1] [�Һ� 2]
    for (i = 0; i < SAMPLES; i++)
    {
        ADC_value[0][i] = adc_once(ADC_P11, ADC_12BIT);
        ADC_value[1][i] = adc_once(ADC_P05, ADC_12BIT);
        ADC_value[2][i] = adc_once(ADC_P00, ADC_12BIT);
        ADC_value[3][i] = adc_once(ADC_P13, ADC_12BIT);
        ADC_value[4][i] = adc_once(ADC_P06, ADC_12BIT);
        ADC_value[5][i] = adc_once(ADC_P01, ADC_12BIT);
    }
}

void Magnet_ADC_Filter(void)
{
    int16 i, j;
    int16 ADC_median_value[INDUCTOR], ADC_sum[INDUCTOR];
    int16 ADC_old_filtered_value[INDUCTOR], ADC_new_filtered_value[INDUCTOR];

    Magnet_ADC_Read();

    /*=========================ð����������==========================*/
    Bubble_Sort_ADC();

    /*===========================��ֵ�˲�=================================*/
    for (i = 0; i < INDUCTOR; i++)
    {
        for (j = 1; j < SAMPLES - 1; j++) // ��ȥ��������С��ĺ�
        {
            ADC_sum[i] += ADC_value[i][j];
        }

        ADC_median_value[i] = ADC_sum[i] / (SAMPLES - 2);
    }

    for (i = 0; i < INDUCTOR; i++)
    {
        ADC_single_divided_value[i] = (int16)(ADC_median_value[i] / 10 * 10); // ����ֵ�и�λ������

        // �ɼ��ݶ�ƽ����ÿ�βɼ����仯40
        ADC_old_filtered_value[i] = ADC_filtered_value[i];
        ADC_new_filtered_value[i] = ADC_single_divided_value[i];

        if (ADC_new_filtered_value[i] >= ADC_old_filtered_value[i])
        {
            ADC_filtered_value[i] = ((ADC_new_filtered_value[i] - ADC_old_filtered_value[i]) > 50 ? (ADC_old_filtered_value[i] + 50) : ADC_new_filtered_value[i]);
        }
        else
        {
            ADC_filtered_value[i] = ((ADC_new_filtered_value[i] - ADC_old_filtered_value[i]) < -60 ? (ADC_old_filtered_value[i] - 60) : ADC_new_filtered_value[i]);
        }
    }
}

void Bubble_Sort_ADC(void)
{
    int i, j, k;
    uint8 swapped;
    int16 temp;

    for (i = 0; i < INDUCTOR; i++)
    {
        swapped = 0; // ÿ������ʼǰ�����δ��������

        for (j = 0; j < SAMPLES - 1; j++)
        {
            for (k = 0; k < SAMPLES - 1 - j; k++)
            {
                if (ADC_value[i][k] > ADC_value[i][k + 1])
                {
                    temp = ADC_value[i][k];
                    ADC_value[i][k] = ADC_value[i][k + 1];
                    ADC_value[i][k + 1] = temp;
                    swapped = 1; // �����˽��������±��
                }
            }
            if (!swapped)
            { // ���һ��ѭ��û�з���������˵������������
                break;
            }
        }
    }
}