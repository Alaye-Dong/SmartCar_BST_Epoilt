#include "gyro.h"

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

void IMU_Get_Data(void)
{
    imu660ra_get_gyro();
    imu660ra_get_acc();
}

#define SAMPLES 5        // ������
#define EXTREME_NUMBER 1 // ������������Сֵ�ĸ���
void IMU_Get_Gyro_Z_Filter(void)
{
    int16 gyro_array[SAMPLES];
    uint8 i;

    for (i = 0; i < SAMPLES; i++)
    {
        imu660ra_get_gyro();
        gyro_array[i] = imu660ra_gyro_z;
    }

    Insertion_Sort(gyro_array, SAMPLES);

    Trimmed_Mean_Filter(gyro_array, SAMPLES, EXTREME_NUMBER, &gyro_z_filtered);
}

void IMU_Get_Yaw_Angle()
{
    yaw_angle += imu660ra_gyro_transition(gyro_z_filtered) * 0.005; // ���ֽǶ�=���ٶ�*����ʱ��
}

void IMU_Yaw_Angle_Get_Control(uint8 mod)
{
    if (mod == 0)
    {
        yaw_angle = 0;
    }
    else if (mod == 1)
    {
        IMU_Get_Yaw_Angle();
    } 
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


/**
 * @description: ��֦ƽ����ȥ������ֵ��ȡƽ��ֵ
 * @param {int16} data_array ����������
 * @param {int16} size
 * @param {int16} extreme_number
 * @param {int16*} filtered_value ������������Сֵ�ĸ���
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