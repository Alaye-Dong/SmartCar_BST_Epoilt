#include "magnet.h"

#define INDUCTOR 6 // ��еĸ���
#define SAMPLES 5  // ���βɼ�����

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
    int16 ADC_old_filtered_value[INDUCTOR];

    // ð������
    Bubble_Sort_ADC();

    for (i = 0; i < INDUCTOR; i++)
    {
        ADC_old_filtered_value[i] = ADC_filtered_value[i];

        // ��ֵ��ֵ�˲�
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
//��S бV ��V 
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
		leftP =  (float)(diangan[0] - 1.0) / (1730.0 - 10.0) * 100.0;		// ��й�һ��
				
		leftV = (float)(diangan[2] - 1.0) / (2150.0 - 10.0) * 100.0;		// ��й�һ��!!!2500
		rightV = (float)(diangan[3] - 1.0) / (2400.0 - 10.0) * 100.0;		// ��й�һ��	����Խ��Խ���� ��ֵ��С 2400 3150
		
    leftS = (float)(diangan[4] - 1.0) / (2280.0 - 10.0) * 100.0; //xie (3200 OK)
		rightP =  (float)(diangan[1] - 1.0) / (2280.0 - 10.0) * 100.0;		// ��й�һ��xie(3200 OK)

		
	  (ADC_filtered_value[0]) = (leftP < 0) ? 0 : leftP;           //��һ���������������
		(ADC_filtered_value[0]) = (leftP > 100) ? 100 : leftP;				//��һ���������������
		(ADC_filtered_value[2]) = (rightV < 0) ? 0 : rightV;					//��һ���������������
		(ADC_filtered_value[2]) = (rightV > 100) ? 100 : rightV;			//��һ���������������
	  (ADC_filtered_value[3]) = (leftV < 0) ? 0 : leftV;						//��һ���������������
		(ADC_filtered_value[3]) = (leftV > 100) ? 100 : leftV;				//��һ���������������
		(ADC_filtered_value[1]) = (rightP < 0) ? 0 : rightP;					//��һ���������������
		(ADC_filtered_value[1]) = (rightP > 100) ? 100 : rightP;			//��һ���������������		
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
