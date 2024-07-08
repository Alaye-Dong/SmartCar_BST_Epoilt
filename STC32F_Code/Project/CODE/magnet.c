#include "magnet.h"

int16 ADC_values[INDUCTORS][SAMPLES] = {0};
int16 ADC_filtered_value[INDUCTORS] = {0};
int16 inductor[INDUCTORS] = {0};

int16 position_vector_modulus = 0;
int16 position_difference_weighted = 0;
int16 position_normal = 0;
int16 position = 0; // position����0��ʾ��ƫ��Ӧ��ת��С��0��ʾ��ƫ��Ӧ��ת

InductorMathTypeDef inductor_math;

void Magnet_ADC_Init(void)
{
    /* ADC���ų�ʼ�� */
    adc_init(ADC_P06, ADC_SYSclk_DIV_2);
    adc_init(ADC_P11, ADC_SYSclk_DIV_2);
    adc_init(ADC_P14, ADC_SYSclk_DIV_2);
    adc_init(ADC_P15, ADC_SYSclk_DIV_2);
    // б������

    // /* ��л�ȡ�Լ� */
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
    Inductor_Math_Pre_Calculate(&inductor_math);
}

void Magnet_ADC_Read(void)
{
    uint8 i;

    for (i = 0; i < SAMPLES; i++)
    {
        ADC_values[LEFT_H][i] = adc_once(ADC_P06, ADC_12BIT);
        ADC_values[LEFT_V][i] = adc_once(ADC_P11, ADC_12BIT);
        ADC_values[LEFT_S][i] = 0; // ��ʱ��ʹ��б��

        ADC_values[RIGHT_S][i] = 0; // ��ʱ��ʹ��б��
        ADC_values[RIGHT_V][i] = adc_once(ADC_P14, ADC_12BIT);
        ADC_values[RIGHT_H][i] = adc_once(ADC_P15, ADC_12BIT);
    }
}

void Magnet_ADC_Filter(void)
{
    uint8 inductor_index;
    int16 ADC_mean_value[INDUCTORS];
    // static int16 ADC_old_filtered_value[INDUCTORS] = {0};

    // // ð������
    // Bubble_Sort_ADC();

    for (inductor_index = 0; inductor_index < INDUCTORS; inductor_index++)
    {
        // ADC_old_filtered_value[inductor_index] = ADC_filtered_value[inductor_index];

        // Trimmed_Mean_Filter(&ADC_values[i], INDUCTORS, EXTREME_NUMBER, &ADC_mean_value[i]);
        Fast_De_Extremum_Averaging(&ADC_values[inductor_index], SAMPLES, &ADC_mean_value[inductor_index]);

        ADC_filtered_value[inductor_index] = (int16)(ADC_mean_value[inductor_index] / 10 * 10); // ����ֵ�и�λ������

        // // �ݶ�ƽ��
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

// void Bubble_Sort_ADC(void)
// {
//     uint8 inductor_index;
//     for (inductor_index = 0; inductor_index < INDUCTORS; inductor_index++)
//     {
//         Bubble_Sort_Int16(ADC_values[inductor_index], SAMPLES);
//     }
// }

// ��й�һ��
#define LEFT_V_MAX 3600 // 4600 5000
#define LEFT_V_MIN 25
#define RIGHT_V_MAX 3600
#define RIGHT_V_MIN 1
#define LEFT_H_MAX 2300
#define LEFT_H_MIN 35
#define RIGHT_H_MAX 2300
#define RIGHT_H_MIN 15
void Inductor_Normal(void)
{
    int16 inductor_normal_value[INDUCTORS] = {0};
    // ��һ����ʽ (x - min) / (max - min) * 100

    inductor_normal_value[LEFT_V] = (float)(ADC_filtered_value[LEFT_V] - LEFT_V_MIN) / (LEFT_V_MAX - LEFT_V_MIN) * 100;
    inductor_normal_value[RIGHT_V] = (float)(ADC_filtered_value[RIGHT_V] - RIGHT_V_MIN) / (RIGHT_V_MAX - RIGHT_V_MIN) * 100;
    inductor_normal_value[LEFT_H] = (float)(ADC_filtered_value[LEFT_H] - LEFT_H_MIN) / (LEFT_H_MAX - LEFT_H_MIN) * 100;
    inductor_normal_value[RIGHT_H] = (float)(ADC_filtered_value[RIGHT_H] - RIGHT_H_MIN) / (RIGHT_H_MAX - RIGHT_H_MIN) * 100;

    inductor[LEFT_V] = FUNC_LIMIT_AB(inductor_normal_value[LEFT_V], 0, 200); // �Ŵ��һ����Χ��200����ֹ��ֱ�����ǰ�������ƫ��������
    inductor[RIGHT_V] = FUNC_LIMIT_AB(inductor_normal_value[RIGHT_V], 0, 200);
    inductor[LEFT_H] = FUNC_LIMIT_AB(inductor_normal_value[LEFT_H], 0, 100);
    inductor[RIGHT_H] = FUNC_LIMIT_AB(inductor_normal_value[RIGHT_H], 0, 100);

    // inductor_normal_value[LEFT_S] = (float)(ADC_filtered_value[LEFT_S] - 10.0) / (2280.0 - 10.0) * 100.0;
    // inductor_normal_value[RIGHT_S] = (float)(ADC_filtered_value[RIGHT_S] - 10.0) / (2280.0 - 10.0) * 100.0;
    // inductor[LEFT_S] = FUNC_LIMIT_AB(inductor_normal_value[LEFT_S], 0, 100);
    // inductor[RIGHT_S] = FUNC_LIMIT_AB(inductor_normal_value[RIGHT_S], 0, 100);
}

void Inductor_Math_Pre_Calculate(InductorMathTypeDef *sptr)
{
    sptr->H_difference = inductor[LEFT_H] - inductor[RIGHT_H];
    sptr->H_sum = inductor[LEFT_H] + inductor[RIGHT_H];

    sptr->V_difference = inductor[LEFT_V] - inductor[RIGHT_V];
    sptr->V_sum = inductor[LEFT_V] + inductor[RIGHT_V];

    sptr->S_difference = inductor[LEFT_S] - inductor[RIGHT_S];
    sptr->S_sum = inductor[LEFT_S] + inductor[RIGHT_S];
}

#define V_GAIN 2 // ��ֱ��������� 2.5
#define H_GAIN 1
void Position_Analyse(void)
{
    int16 temp_left = 0;
    int16 temp_right = 0;

    int16 temp_difference = 0;
    int16 temp_sum_difference_weighted = 0;

    int16 temp_sum = 0;

    // ! ����ģ��Ⱥ��㷨
    temp_left = My_Sqrt(inductor[LEFT_H] * inductor[LEFT_H] + inductor[LEFT_V] * inductor[LEFT_V]); // TODO ���Զ�����м�Ȩ��Ȩ
    temp_right = My_Sqrt(inductor[RIGHT_H] * inductor[RIGHT_H] + inductor[RIGHT_V] * inductor[RIGHT_V]);
    position_vector_modulus = (temp_left - temp_right) * 100 / (temp_left + temp_right + 1); // ������Ⱥͣ���1��ֹ��ĸΪ0 //TODO : ���Ըĳɲ��Ȩ�᲻�������

    // * ��ȺͲ��Ȩ�㷨
    temp_difference = H_GAIN * (inductor_math.H_difference) + V_GAIN * (inductor_math.V_difference); // TODO ��ֱ��б���ƫ��ʧ������
    temp_sum_difference_weighted = H_GAIN * (inductor_math.H_sum) + abs((inductor_math.V_difference));
    position_difference_weighted = (temp_difference * 100) / (temp_sum_difference_weighted + 1);

    //  ��ͨ��Ⱥ�
    temp_sum = (inductor_math.H_sum) + (inductor_math.V_difference);
    position_normal = (temp_difference * 100) / (temp_sum + 1);

    position = position_difference_weighted;

    Position_Loss_Remember_Protect(1);
}

/**
 * @description: ���߼����Ǽ�����ͣ��
 * @param {uint8} protect_mode Ϊ1ʱ��������ͣ������ 0ʱ�ر�
 * @return {*}
 */
uint16 position_loss_timer = 0;
uint8 position_loss_stop_protect_flag = 0;
void Position_Loss_Remember_Protect(uint8 protect_mode)
{
    static uint16 position_remember = 0;

    if (position_loss_timer == 0 && (inductor[LEFT_H] + inductor[RIGHT_H] <= 5) &&
        (inductor[LEFT_H] <= 2 || inductor[RIGHT_H] <= 2) &&
        (inductor[LEFT_V] <= 2 || inductor[RIGHT_V] <= 2) &&
        (inductor[LEFT_V] + inductor[RIGHT_V] <= 5)) // �״μ�⵽���ߣ���¼��ǰλ�õ�ƫ��
    {
        position_remember = position;
        position_loss_timer++;
    }

    if (position_loss_timer != 0 && (inductor[LEFT_V] + inductor[RIGHT_V] + inductor[LEFT_H] + inductor[RIGHT_H] <= 20)) // ���ڶ���״̬��ʹ�ü�����
    {
        position_loss_timer++;
        position = position_remember;
        BEEP_ON();
    }
    else // �Ƕ���״̬���������
    {
        position_loss_timer = 0;
        BEEP_OFF();
    }

    if (protect_mode == 1 && position_loss_timer > TIMER_COUNT_MS(2000)) // �����ۼ� 400 * 5ms = 2s ͣ������
    {
        position_loss_stop_protect_flag = 1;
        position = 0;
        BEEP_OFF();
    }
}
