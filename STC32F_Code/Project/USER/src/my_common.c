#include "my_common.h"

// ð��������
void Bubble_Sort_Int(int16 data_array[], int16 length)
{
    int16 i, j;
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
 * @param {int16} size ����Ĵ�С
 * @param {int16} extreme_number ������������Сֵ�ĸ���
 * @param {int16*} filtered_value �õ���ƽ��ֵ
 * @return {*}
 */
void Trimmed_Mean_Filter(int16 data_array[], int16 size, int16 extreme_number, int16* filtered_value)
{
    int16 temp_sum = 0;
    uint8 i;

    for (i = extreme_number; i < size - extreme_number; i++) // ��ȥ��������С���ĺ�
    {
        temp_sum += data_array[i];
    }

    *filtered_value = temp_sum / (size - extreme_number * 2); //����ȡƽ��
}

// �Ż����Ĳ�������
void Insertion_Sort(int16 array[], int16 size)
{
    int16 i, j;
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