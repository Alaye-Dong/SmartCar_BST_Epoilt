#include "my_common.h"

int8 element_busy_flag = 0; //赛道元素检测忙标志位。防止在成功元素检测后进行对应动作过程中检测到新的赛道元素，导致运行错乱。

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

/**
 * @description: 低通滤波
 * @note Y(n)=αX(n) + (1-α)Y(n-1) 式中：α=滤波系数；X(n)=本次采样值；Y(n-1)=上次滤波输出值；Y(n)=本次滤波输出值。
 * @param {float} fliter_factor 滤波系数
 * @param {int16} last_value
 * @param {int16} new_value
 * @return {*}
 */
int16 Low_Pass_Filter(int16 new_value, int16 last_value, float fliter_factor)
{
    return (int16)(fliter_factor * new_value + (1 - fliter_factor) * last_value);
}

// 快速平方根算法
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
    long i = *(long *)&x;           // get bits for floating VALUE
    i = (0x5f375a86) - (i >> 1);    // gives initial guess y0
    x = *(float *)&i;               // convert bits BACK to float
    x = x * (1.5f - xhalf * x * x); // Newton step, repeating increases accuracy
    return x;
}

/**
 * @description: 闵可夫斯基距离计算函数 在电磁车中用于计算目标电感相似度
 * @param {int} inductor 待比较的数组
 * @param {int} target 目标数组
 * @param {int} size 数组大小
 * @param {int} p 闵可夫斯基距离的参数：p=1时为曼哈顿距离 p=2时为欧几里得距离
 * @return {*}
 * @note: p=1时为曼哈顿距离,这适用于处理离散数据或需要考虑各个维度的线性关系的情况。
 *        p=2时为欧几里得距离,这适用于处理连续数据或需要考虑各个维度的平方关系的情况，
 *        当p较小时，距离主要由离群点决定；当p较大时，距离主要由较大差值的维度决定。
 */
float Calculate_Minkowski_Distance(int16 inductor[], int16 target[], uint8 size, uint8 p)
{
    uint8 i;
    float distance = 0.0;

    // 计算每个传感器值与目标值的差的绝对值的 p 次方，并求和
    for (i = 0; i < size; i++)
    {
        distance += pow(FUNC_ABS(inductor[i] - target[i]), p);
    }

    // 对求和结果取 p 次方根
    // distance = pow(distance, 1.0 / p); // 不计算p次根，降低计算压力，结果不再是标准的闵可夫斯基距离，但仍可以进行相似度比较

    return distance;
}