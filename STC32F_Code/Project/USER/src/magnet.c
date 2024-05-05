#include "magnet.h"

/**********全局变量定义********/

float g_fDirectionError[5];     // 方向偏差（g_fDirectionError[0]为一对水平电感的差比和偏差）
float g_fDirectionError_dot[3]; // 方向偏差微分（g_fDirectionError_dot[0]为一对水平电感的差比和偏差微分）

int16 g_ValueOfAD[6] = {0}; // 获取的电感值
int16 g_ValueOfADFilter[5] = {0};

// 定义数组大小常量，提高可读性和可维护性
#define INDUCTOR 6  //电感的个数
#define SAMPLES 5  //单次采集次数
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
    int16 i, j, k, temp;
    int16 ad_valu1[INDUCTOR], ad_sum[INDUCTOR];
    int16 ValueOfADOld[INDUCTOR], ValueOfADNew[INDUCTOR];

    Magnet_ADC_Read();

    /*=========================冒泡排序升序==========================*/ // 舍弃最大值和最小值
    // 调用排序函数，对每一行进行排序
    for (int i = 0; i < INDUCTOR; i++) {
        BubbleSortRow(ADC_value[i]);
    }

    /*===========================中值滤波=================================*/
    for (i = 0; i < 6; i++) // 求中间三项的和
    {
        ad_sum[i] = ADC_value[i][1] + ADC_value[i][2] + ADC_value[i][3];
        ad_valu1[i] = ad_sum[i] / 3;
    }

    for (i = 0; i < 6; i++) // 将数值中个位数除掉
    {
        g_ValueOfAD[i] = (int16)(ad_valu1[i] / 10 * 10);

        // 采集梯度平滑，每次采集最大变化40
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
        // 设置标志位判断是否需要继续排序
        uint8 swapped = 0;
        for (j = 0; j < SAMPLES - 1; j++) {
            if (ADC_value[i][j] > ADC_value[i][j + 1]) {
                temp = ADC_value[i][j + 1];
                ADC_value[i][j + 1] = ADC_value[i][j];
                ADC_value[i][j] = temp;
                swapped = 1; // 标志位设置为1，表示发生过交换
            }
        }
        // 如果没有发生交换，说明数组已排序完成，提前结束循环
        if (!swapped) {
            break;
        }
    }
}