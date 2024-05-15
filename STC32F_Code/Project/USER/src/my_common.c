#include "my_common.h"

// 冒泡排序函数
void Bubble_Sort_Int16(int16 data_array[], uint8 length)
{
    uint8 i, j;
    uint8 swapped;

    for (i = 0; i < length - 1; i++)
    {
        swapped = 0; // 每轮排序开始前，标记未发生交换

        for (j = 0; j < length - 1 - i; j++)
        {
            if (data_array[j] > data_array[j + 1])
            {
                int16 temp = data_array[j];
                data_array[j] = data_array[j + 1];
                data_array[j + 1] = temp;
                swapped = 1; // 发生了交换，更新标记
            }
        }

        if (!swapped)
        {
            // 如果一轮循环没有发生交换，说明数组已排序
            break;
        }
    }
}

/**
 * @description: 剪枝平均，去除极端值后取平均值
 * @param {int16} data_array 数组已有序
 * @param {uint8} size 数组的大小
 * @param {uint8} extreme_number 舍弃的最大或最小值的个数
 * @param {int16*} filtered_value 得到的平均值
 * @return {*}
 */
void Trimmed_Mean_Filter(int16 data_array[], uint8 size, uint8 extreme_number, int16 *filtered_value)
{
    int16 temp_sum = 0;
    uint8 i;

    for (i = extreme_number; i < size - extreme_number; i++) // 求去除最大和最小项后的和
    {
        temp_sum += data_array[i];
    }

    *filtered_value = temp_sum / (size - extreme_number * 2); // 其余取平均
}

// 优化过的插入排序
void Insertion_Sort(int16 array[], uint8 size)
{
    uint8 i, j;
    int16 key;

    for (i = 1; i < size; i++)
    {
        key = array[i];
        j = i - 1;

        /* 将大于key的元素向后移动 */
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

//快速平方根算法
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

// 快速倒数平方根算法
float Inv_Sqrt(float x)
{
    float xhalf = 0.5f * x;
    long i = *(long *)&x;             // get bits for floating VALUE
    i = (0x5f375a86) - (i >> 1);      // gives initial guess y0
    x = *(float *)&i;               // convert bits BACK to float
    x = x * (1.5f - xhalf * x * x); // Newton step, repeating increases accuracy
    return x;
}