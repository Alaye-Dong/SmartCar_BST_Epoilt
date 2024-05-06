#include "magnet.h"

#define INDUCTOR 6 // 电感的个数
#define SAMPLES 5  // 单次采集次数

int16 ADC_value[INDUCTOR][SAMPLES] = {0};
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

void Magnet_Process(void)
{
    Magnet_ADC_Read();
    Magnet_ADC_Filter();
}

void Magnet_ADC_Read(void)
{
    uint8 i;
    // 牛爷爷的车队 [左横 3] [左斜 4] [左竖 0] || [右竖 5] [右斜 1] [右横 2]
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
    int16 ADC_old_filtered_value[INDUCTOR];

    // 冒泡排序
    Bubble_Sort_ADC();

    for (i = 0; i < INDUCTOR; i++)
    {
        ADC_old_filtered_value[i] = ADC_filtered_value[i];

        // 中值均值滤波
        for (j = 1; j < SAMPLES - 1; j++) // 求去除最大和最小项的和
        {
            ADC_sum[i] += ADC_value[i][j];
        }
        ADC_median_value[i] = ADC_sum[i] / (SAMPLES - 2);

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
//横S 斜V 竖V 
unsigned int data diangan[9];
short leftP = 0, leftV = 0, rightV = 0, rightP = 0, leftS = 0, rightS = 0, middle = 0, second_leftp = 0, second_rightp = 0;
void Inductor_Normal(void)
{
    Magnet_ADC_Read();
    Magnet_ADC_Filter();
    
    diangan[0]=ADC_filtered_value[0];    	
    diangan[1]=ADC_filtered_value[1];			
    diangan[2]=ADC_filtered_value[2]; 		
    diangan[3]=ADC_filtered_value[3];
    diangan[4]=ADC_filtered_value[4];
    diangan[5]=ADC_filtered_value[5];	
//	  diangan[6]=ADC_filtered_value[6];	
//	  diangan[7]=ADC_filtered_value[7];
//    diangan[8]=ADC_filtered_value[8];	
		rightS = (float)(diangan[5] - 1.0) / (1640.0 - 10.0) * 100.0;	
		leftP =  (float)(diangan[0] - 1.0) / (1730.0 - 10.0) * 100.0;		// 电感归一化
				
		leftV = (float)(diangan[2] - 1.0) / (2150.0 - 10.0) * 100.0;		// 电感归一化!!!2500
		rightV = (float)(diangan[3] - 1.0) / (2400.0 - 10.0) * 100.0;		// 电感归一化	除数越大越往左 右值变小 2400 3150
		
    leftS = (float)(diangan[4] - 1.0) / (2280.0 - 10.0) * 100.0; //xie (3200 OK)
		rightP =  (float)(diangan[1] - 1.0) / (2280.0 - 10.0) * 100.0;		// 电感归一化xie(3200 OK)

		
	  (ADC_filtered_value[0]) = (leftP < 0) ? 0 : leftP;           //归一化后限制输出幅度
		(ADC_filtered_value[0]) = (leftP > 100) ? 100 : leftP;				//归一化后限制输出幅度
		(ADC_filtered_value[2]) = (rightV < 0) ? 0 : rightV;					//归一化后限制输出幅度
		(ADC_filtered_value[2]) = (rightV > 100) ? 100 : rightV;			//归一化后限制输出幅度
	  (ADC_filtered_value[3]) = (leftV < 0) ? 0 : leftV;						//归一化后限制输出幅度
		(ADC_filtered_value[3]) = (leftV > 100) ? 100 : leftV;				//归一化后限制输出幅度
		(ADC_filtered_value[1]) = (rightP < 0) ? 0 : rightP;					//归一化后限制输出幅度
		(ADC_filtered_value[1]) = (rightP > 100) ? 100 : rightP;			//归一化后限制输出幅度		
    (ADC_filtered_value[4]) = (leftS < 0) ? 0 : leftS;
	  (ADC_filtered_value[4]) = (leftS > 100) ? 100 : leftS;
		(ADC_filtered_value[5]) = (rightS < 0) ? 0 : rightS;
	  (ADC_filtered_value[5]) = (rightS > 100) ? 100 : rightS;
//		(ADC_filtered_value[6]) = (middle < 0) ? 0 : middle;
//	  (ADC_filtered_value[6]) = (middle > 100) ? 100 : middle;
//		(ADC_filtered_value[7]) = (second_leftp < 0) ? 0 : second_leftp;
//	  (ADC_filtered_value[7]) = (second_leftp > 100) ? 100 : second_leftp;
//		(ADC_filtered_value[8]) = (second_rightp < 0) ? 0 : second_rightp;
//	  (ADC_filtered_value[8]) = (second_rightp > 100) ? 100 : second_rightp;
}
