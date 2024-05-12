#include "gyro.h"
// int16 imu660ra_gyro_x = 0, imu660ra_gyro_y = 0, imu660ra_gyro_z = 0;            // ��������������   gyro (������)
// int16 imu660ra_acc_x = 0, imu660ra_acc_y = 0, imu660ra_acc_z = 0;               // ������ٶȼ����� acc  (accelerometer ���ٶȼ�)

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

#define SAMPLES 5        // ������
#define EXTREME_NUMBER 1 // ������������Сֵ�ĸ���
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

// Yaw���ٶ�,ʹ��ʱҪȷ��IMU_Gyro_Z_Filter()���жϻ���ѭ����
float IMU_Get_Yaw_Rad_S(void)
{
    return imu660ra_gyro_transition(gyro_z_filtered);
}

void IMU_Get_Yaw_Angle()
{
    yaw_angle += IMU_Get_Yaw_Rad_S() * 0.005; // ���ֽǶȣ����ٶ�*����ʱ��10ms
}

// �Ż����Ĳ�������
void insertion_sort(int16 array[], int16 size)
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

//��֦ƽ����ȥ������ֵ��ȡƽ��ֵ
void Trimmed_Mean_Filter(int16 data_array[], int16 size, int16 extreme_filter, int16* filtered_value)
{
    int16 temp_sum = 0;
    uint8 i;
    //data_array����������exterme_filterΪ������������Сֵ�ĸ���

    for (i = extreme_filter; i < size - extreme_filter; i++) // ��ȥ��������С���ĺ�
    {
        temp_sum += data_array[i];
    }

    *filtered_value = temp_sum / (size - extreme_filter * 2); //����ȡƽ��
}