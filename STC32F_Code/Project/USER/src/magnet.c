#include "magnet.h"

#define INDUCTORS 6 // ��еĸ���
#define SAMPLES 5   // ���βɼ�����

// ˮƽ 32��H������ֱ 05��V����б�� 41��S��
#define left_V 0
#define right_V 5
#define left_H 3
#define right_H 2
#define left_S 4
#define right_S 1

int16 ADC_value[INDUCTORS][SAMPLES] = {0};
int16 ADC_filtered_value[INDUCTORS] = {0};
int16 inductor_normal_value[INDUCTORS] = {0};
int16 inductor_left_V, inductor_right_V, inductor_left_H, inductor_right_H, inductor_left_S, inductor_right_S = 0;
int16 position_left, position_right, position = 0; //position����0��ʾ��ƫ��Ӧ��ת��С��0��ʾ��ƫ��Ӧ��ת

void Magnet_ADC_Init(void)
{
    adc_init(ADC_P00, ADC_SYSclk_DIV_2);
    adc_init(ADC_P01, ADC_SYSclk_DIV_2);
    adc_init(ADC_P05, ADC_SYSclk_DIV_2);
    adc_init(ADC_P06, ADC_SYSclk_DIV_2);
    adc_init(ADC_P11, ADC_SYSclk_DIV_2);
    adc_init(ADC_P13, ADC_SYSclk_DIV_2);
}

void Inductor_Process(void)
{
    Magnet_ADC_Read();
    Magnet_ADC_Filter();
    Inductor_Normal();
    Inductor_Analyse();
}

void Magnet_ADC_Read(void)
{
    uint8 i;
    // ţүү�ĳ��� [��� 3] [��б 4] [���� 0] || [���� 5] [��б 1] [�Һ� 2]
    for (i = 0; i < SAMPLES; i++)
    {
        ADC_value[left_V][i] = adc_once(ADC_P11, ADC_12BIT);
        ADC_value[right_S][i] = adc_once(ADC_P05, ADC_12BIT);
        ADC_value[right_H][i] = adc_once(ADC_P00, ADC_12BIT);
        ADC_value[left_H][i] = adc_once(ADC_P13, ADC_12BIT);
        ADC_value[left_S][i] = adc_once(ADC_P06, ADC_12BIT);
        ADC_value[right_V][i] = adc_once(ADC_P01, ADC_12BIT);
    }
}

void Magnet_ADC_Filter(void)
{
    int16 i, j;
    int16 ADC_median_value[INDUCTORS], ADC_sum[INDUCTORS];
    int16 ADC_old_filtered_value[INDUCTORS];

    // ð������
    Bubble_Sort_ADC();

    for (i = 0; i < INDUCTORS; i++)
    {
        ADC_old_filtered_value[i] = ADC_filtered_value[i];

        // ��λֵƽ���˲�
        for (j = 1; j < SAMPLES - 1; j++) // ��ȥ��������С��ĺ�
        {
            ADC_sum[i] += ADC_value[i][j];
        }
        ADC_median_value[i] = ADC_sum[i] / (SAMPLES - 2);

        ADC_filtered_value[i] = (int16)(ADC_median_value[i] / 10 * 10); // ����ֵ�и�λ������

        // �ݶ�ƽ��
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
    int i, j, k;
    uint8 swapped;
    int16 temp;

    for (i = 0; i < INDUCTORS; i++)
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


void Inductor_Normal(void)
{
    // (x - min) / (max - min) * 100
    inductor_normal_value[left_V] = (float)(ADC_filtered_value[left_V] - 1.0) / (1730.0 - 10.0) * 100.0; // ��й�һ��
    inductor_normal_value[right_V] = (float)(ADC_filtered_value[right_V] - 1.0) / (1640.0 - 10.0) * 100.0;

    inductor_normal_value[left_H] = (float)(ADC_filtered_value[left_H] - 1.0) / (2400.0 - 10.0) * 100.0;   // ��й�һ��	����Խ��Խ���� ��ֵ��С 2400 3150
    inductor_normal_value[right_H] = (float)(ADC_filtered_value[right_H] - 1.0) / (2150.0 - 10.0) * 100.0; // ��й�һ��!!!2500

    inductor_normal_value[left_S] = (float)(ADC_filtered_value[left_S] - 1.0) / (2280.0 - 10.0) * 100.0;   // xie (3200 OK)
    inductor_normal_value[right_S] = (float)(ADC_filtered_value[right_S] - 1.0) / (2280.0 - 10.0) * 100.0; // ��й�һ��xie(3200 OK)

    inductor_left_V = (inductor_normal_value[left_V] < 0) ? 0 : inductor_normal_value[left_V];
    inductor_left_V = (inductor_normal_value[left_V] > 100) ? 100 : inductor_normal_value[left_V];
    inductor_right_V = (inductor_normal_value[right_V] < 0) ? 0 : inductor_normal_value[right_V];
    inductor_right_V = (inductor_normal_value[right_V] > 100) ? 100 : inductor_normal_value[right_V];

    inductor_left_H = (inductor_normal_value[left_H] < 0) ? 0 : inductor_normal_value[left_H];
    inductor_left_H = (inductor_normal_value[left_H] > 100) ? 100 : inductor_normal_value[left_H];
    inductor_right_H = (inductor_normal_value[right_H] < 0) ? 0 : inductor_normal_value[right_H];
    inductor_right_H = (inductor_normal_value[right_H] > 100) ? 100 : inductor_normal_value[right_H];

    inductor_left_S = (inductor_normal_value[left_S] < 0) ? 0 : inductor_normal_value[left_S];
    inductor_left_S = (inductor_normal_value[left_S] > 100) ? 100 : inductor_normal_value[left_S];
    inductor_right_S = (inductor_normal_value[right_S] < 0) ? 0 : inductor_normal_value[right_S];
    inductor_right_S = (inductor_normal_value[right_S] > 100) ? 100 : inductor_normal_value[right_S];
}

void Inductor_Analyse(void)
{
    position_left = sqrt(inductor_left_H * inductor_left_H + inductor_left_V * inductor_left_V);
    position_right = sqrt(inductor_right_H * inductor_right_H + inductor_right_V * inductor_right_V);
    position = (position_left - position_right) * 100 / (position_left + position_right + 1); // ��1��ֹ��ĸΪ0
}

// static float g_fDirectionErrorTemp[2][5];
// InductorNormal();
// g_fDirectionError[0] = (float)(sqrt(g_ValueOfAD[2]) - sqrt(g_ValueOfAD[3])) * 100 / (sqrt(g_ValueOfAD[3]) + sqrt(g_ValueOfAD[2])); // ˮƽ��еĲ�Ⱥ���Ϊƫ��Ϊƫ��
// g_fDirectionError[1] = (float)(sqrt(g_ValueOfAD[5]) - sqrt(g_ValueOfAD[0])) * 100 / (sqrt(g_ValueOfAD[5]) + sqrt(g_ValueOfAD[0]));
// g_fDirectionError[2] = (float)(0.8 * (sqrt(g_ValueOfAD[2]) - sqrt(g_ValueOfAD[3])) + 0.4 * (sqrt(g_ValueOfAD[4]) - sqrt(g_ValueOfAD[1]))) * 100 /
//                        (0.8 * (sqrt(g_ValueOfAD[3]) + sqrt(g_ValueOfAD[2])) + 0.4 * abs(sqrt(g_ValueOfAD[4]) - sqrt(g_ValueOfAD[1])));
// g_fDirectionError[3] = (float)(g_ValueOfAD[5] - g_ValueOfAD[0]);

// g_fDirectionErrorTemp[0][4] = g_fDirectionErrorTemp[0][3];
// g_fDirectionErrorTemp[0][3] = g_fDirectionErrorTemp[0][2];
// g_fDirectionErrorTemp[0][2] = g_fDirectionErrorTemp[0][1];
// g_fDirectionErrorTemp[0][1] = g_fDirectionErrorTemp[0][0];
// g_fDirectionErrorTemp[0][0] = g_fDirectionError[0];
// g_fDirectionError_dot[0] = 5 * (g_fDirectionErrorTemp[0][0] - g_fDirectionErrorTemp[0][3]); // ˮƽ��е�ƫ��΢��
