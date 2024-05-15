#include "my_common.h"

// ð��������
void Bubble_Sort_Int16(int16 data_array[], uint8 length)
{
    uint8 i, j;
    uint8 swapped;

    for (i = 0; i < length - 1; i++)
    {
        swapped = 0; // ÿ������ʼǰ�����δ��������

        for (j = 0; j < length - 1 - i; j++)
        {
            if (data_array[j] > data_array[j + 1])
            {
                int16 temp = data_array[j];
                data_array[j] = data_array[j + 1];
                data_array[j + 1] = temp;
                swapped = 1; // �����˽��������±��
            }
        }

        if (!swapped)
        {
            // ���һ��ѭ��û�з���������˵������������
            break;
        }
    }
}

/**
 * @description: ��֦ƽ����ȥ������ֵ��ȡƽ��ֵ
 * @param {int16} data_array ����������
 * @param {uint8} size ����Ĵ�С
 * @param {uint8} extreme_number ������������Сֵ�ĸ���
 * @param {int16*} filtered_value �õ���ƽ��ֵ
 * @return {*}
 */
void Trimmed_Mean_Filter(int16 data_array[], uint8 size, uint8 extreme_number, int16 *filtered_value)
{
    int16 temp_sum = 0;
    uint8 i;

    for (i = extreme_number; i < size - extreme_number; i++) // ��ȥ��������С���ĺ�
    {
        temp_sum += data_array[i];
    }

    *filtered_value = temp_sum / (size - extreme_number * 2); // ����ȡƽ��
}

// �Ż����Ĳ�������
void Insertion_Sort(int16 array[], uint8 size)
{
    uint8 i, j;
    int16 key;

    for (i = 1; i < size; i++)
    {
        key = array[i];
        j = i - 1;

        /* ������key��Ԫ������ƶ� */
        while (j >= 0 && array[j] > key)
        {
            array[j + 1] = array[j];
            j = j - 1;
        }
        array[j + 1] = key;
    }
}

int16 Limit(int16 value, int16 min, int16 max)
{
    if (value < min)
    {
        return min;
    }
    else if (value > max)
    {
        return max;
    }
    else
    {
        return value;
    }
}

//����ƽ�����㷨
float My_Sqrt(float number)
{
    long i;
    float x, y;
    const float f = 1.5F;

    x = number * 0.5F;
    y = number;
    i = *(long *)&y;
    i = 0x5f375a86 - (i >> 1);
    y = *(float *)&i;
    y = y * (f - (x * y * y));
    y = y * (f - (x * y * y));
    return number * y;
}

// ���ٵ���ƽ�����㷨
float Inv_Sqrt(float x)
{
    float xhalf = 0.5f * x;
    long i = *(long *)&x;             // get bits for floating VALUE
    i = (0x5f375a86) - (i >> 1);      // gives initial guess y0
    x = *(float *)&i;               // convert bits BACK to float
    x = x * (1.5f - xhalf * x * x); // Newton step, repeating increases accuracy
    return x;
}