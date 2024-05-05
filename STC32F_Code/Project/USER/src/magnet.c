#include "magnet.h"

/**********ȫ�ֱ�������********/

float g_fDirectionError[5];     // ����ƫ�g_fDirectionError[0]Ϊһ��ˮƽ��еĲ�Ⱥ�ƫ�
float g_fDirectionError_dot[3]; // ����ƫ��΢�֣�g_fDirectionError_dot[0]Ϊһ��ˮƽ��еĲ�Ⱥ�ƫ��΢�֣�

int16 g_ValueOfAD[6] = {0}; // ��ȡ�ĵ��ֵ
int16 g_ValueOfADFilter[5] = {0};

// ���������С��������߿ɶ��ԺͿ�ά����
#define INDUCTOR 6  //��еĸ���
#define SAMPLES 5  //���βɼ�����
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
    // [��� 3] [��б 4] [���� 0] || [���� 5] [��б 1] [�Һ� 2]
    for (i = 0; i < SAMPLES; i++)
    {
        ADC_value[0][i] = adc_once(ADC_P11, ADC_12BIT); // zui you
        ADC_value[1][i] = adc_once(ADC_P05, ADC_12BIT); // zhong jian
        ADC_value[2][i] = adc_once(ADC_P00, ADC_12BIT); // ��
        ADC_value[3][i] = adc_once(ADC_P13, ADC_12BIT); // ��
        ADC_value[4][i] = adc_once(ADC_P06, ADC_12BIT); // zui zuo
        ADC_value[5][i] = adc_once(ADC_P01, ADC_12BIT); // 05     /
    }
}

void Magnet_ADC_Filter(void)
{
    int16 i, j, k, temp;
    int16 ad_valu1[INDUCTOR], ad_sum[INDUCTOR];
    int16 ValueOfADOld[INDUCTOR], ValueOfADNew[INDUCTOR];

    Magnet_ADC_Read();

    /*=========================ð����������==========================*/ // �������ֵ����Сֵ
    // ��������������ÿһ�н�������
    for (int i = 0; i < INDUCTOR; i++) {
        BubbleSortRow(ADC_value[i]);
    }

    /*===========================��ֵ�˲�=================================*/
    for (i = 0; i < 6; i++) // ���м�����ĺ�
    {
        ad_sum[i] = ADC_value[i][1] + ADC_value[i][2] + ADC_value[i][3];
        ad_valu1[i] = ad_sum[i] / 3;
    }

    for (i = 0; i < 6; i++) // ����ֵ�и�λ������
    {
        g_ValueOfAD[i] = (int16)(ad_valu1[i] / 10 * 10);

        // �ɼ��ݶ�ƽ����ÿ�βɼ����仯40
        ValueOfADOld[i] = g_ValueOfADFilter[i];
        ValueOfADNew[i] = g_ValueOfAD[i];

        if (ValueOfADNew[i] >= ValueOfADOld[i])
            g_ValueOfADFilter[i] = ((ValueOfADNew[i] - ValueOfADOld[i]) > 50 ? (ValueOfADOld[i] + 50) : ValueOfADNew[i]);
        else
            g_ValueOfADFilter[i] = ((ValueOfADNew[i] - ValueOfADOld[i]) < -60 ? (ValueOfADOld[i] - 60) : ValueOfADNew[i]);
    }
}

void BubbleSortRow(int16 ADC_value[][SAMPLES]) {
    int i, j, k;
    int16 temp;

    for (i = 0; i < INDUCTOR; i++) {
        // ���ñ�־λ�ж��Ƿ���Ҫ��������
        uint8 swapped = 0;
        for (j = 0; j < SAMPLES - 1; j++) {
            if (ADC_value[i][j] > ADC_value[i][j + 1]) {
                temp = ADC_value[i][j + 1];
                ADC_value[i][j + 1] = ADC_value[i][j];
                ADC_value[i][j] = temp;
                swapped = 1; // ��־λ����Ϊ1����ʾ����������
            }
        }
        // ���û�з���������˵��������������ɣ���ǰ����ѭ��
        if (!swapped) {
            break;
        }
    }
}