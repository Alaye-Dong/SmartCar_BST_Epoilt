#include "gyro.h"
// int16 imu660ra_gyro_x = 0, imu660ra_gyro_y = 0, imu660ra_gyro_z = 0;            // 三轴陀螺仪数据   gyro (陀螺仪)
// int16 imu660ra_acc_x = 0, imu660ra_acc_y = 0, imu660ra_acc_z = 0;               // 三轴加速度计数据 acc  (accelerometer 加速度计)

int16 gyro_z_filtered;
float yaw_angle;

void IMU_Init(void)
{
    if (imu660ra_init())
    {
        lcd_showstr(1 * 8, 1, "GYRO_ERROR");
    }
}

void IMU_Offset(void)
{
}

void IMU_Get_data_array(void)
{
    imu660ra_get_gyro();
    imu660ra_get_acc();
}

#define SAMPLES 5        // 样本数
#define EXTREME_NUMBER 1 // 舍弃的最大或最小值的个数
void IMU_Gyro_Z_Filter()
{
    int16 gyro_array[SAMPLES];
    uint8 i;

    for (i = 0; i < SAMPLES; i++)
    {
        imu660ra_get_gyro();
        gyro_array[i] = imu660ra_gyro_z;
    }

    insertion_sort(gyro_array, SAMPLES);

    Trimmed_Mean_Filter(gyro_array, SAMPLES, EXTREME_NUMBER, &gyro_z_filtered);
}

// Yaw角速度,使用时要确保IMU_Gyro_Z_Filter()在中断或者循环中
float IMU_Get_Yaw_Rad_S(void)
{
    return imu660ra_gyro_transition(gyro_z_filtered);
}

void IMU_Get_Yaw_Angle()
{
    yaw_angle += IMU_Get_Yaw_Rad_S() * 0.005; // 积分角度：角速度*积分时间10ms
}

// 优化过的插入排序
void insertion_sort(int16 array[], int16 size)
{
    int16 i, j;
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

//剪枝平均，去除极端值后取平均值
void Trimmed_Mean_Filter(int16 data_array[], int16 size, int16 extreme_filter, int16* filtered_value)
{
    int16 temp_sum = 0;
    uint8 i;
    //data_array数组已有序，exterme_filter为舍弃的最大或最小值的个数

    for (i = extreme_filter; i < size - extreme_filter; i++) // 求去除最大和最小项后的和
    {
        temp_sum += data_array[i];
    }

    *filtered_value = temp_sum / (size - extreme_filter * 2); //其余取平均
}