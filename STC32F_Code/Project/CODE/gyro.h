#ifndef __GYRO_H_
#define __GYRO_H_

#include "headfile.h"

typedef enum YAW_ANGLE_GET_MOD
{
    RESET = 0, // 重置归零
    ON,        // 运行
    OFF        // 关闭
} YAW_ANGLE_GET_MOD_enmu;

extern int16 gyro_z_filtered;
extern float yaw_angle;
extern YAW_ANGLE_GET_MOD_enmu yaw_angle_get_mod;

void IMU_Init(void);
void IMU_Offset(void);
void IMU_Process(void);
void IMU_Get_Data(void);
void IMU_Get_Gyro_Z_Filter(void);
void IMU_Get_Yaw_Angle(void);
void IMU_Yaw_Angle_Get_Control(YAW_ANGLE_GET_MOD_enmu mod);

#endif