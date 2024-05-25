#include "gyro.h"

int16 gyro_z_filtered;
float yaw_angle;

YAW_ANGLE_GET_MOD_enmu yaw_angle_get_mod = OFF;

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

void IMU_Process(void)
{
    IMU_Get_Gyro_Z_Filter();
    IMU_Yaw_Angle_Get_Control(yaw_angle_get_mod);
}

void IMU_Get_Data(void)
{
    imu660ra_get_gyro();
    imu660ra_get_acc();
}

#define SAMPLES 5        // 样本数
#define EXTREME_NUMBER 1 // 舍弃的最大或最小值的个数
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
    yaw_angle += imu660ra_gyro_transition(gyro_z_filtered) * 0.005; // 积分角度=角速度*积分时间
}

void IMU_Yaw_Angle_Get_Control(YAW_ANGLE_GET_MOD_enmu mod)
{
    switch (mod)
    {
    case RESET:
        yaw_angle = 0;
        break;
    case ON:
        IMU_Get_Yaw_Angle();
        break;
    case OFF:
        break;
    default:
        break;
    }
}