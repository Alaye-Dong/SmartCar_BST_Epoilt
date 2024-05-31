#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "headfile.h"

// PWM通道PIN定义
#define DIR_2 P10 // 右轮
#define DIR_1 P24 // 左轮
#define PWM_2 PWMA_CH2N_P13
#define PWM_1 PWMA_CH4P_P26

// 正面对着电机的安装轴端面，逆时针方向旋转为正转的方向。
// 当DIR引脚为逻辑高时，电机正转；当DR引脚为逻辑低时，电机反转。

extern int16 motor_left_pwm, motor_right_pwm;
extern int16 motor_left_pwm_last, motor_right_pwm_last;
void Motor_PWM_Init(void);
void Motor_PWM_Write(void);

#endif