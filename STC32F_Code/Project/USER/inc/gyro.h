#ifndef _GYRO_H_
#define _GYRO_H_

#include "headfile.h"

extern int16 gyro_z_filtered;
extern float yaw_angle;

void IMU_Init(void);
void IMU_Offset(void);
void IMU_Process(void);
void IMU_Get_Data(void);
void IMU_Get_Gyro_Z_Filter(void);
void IMU_Get_Yaw_Angle(void);
void IMU_Yaw_Angle_Get_Control(uint8 mod);

#endif