#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "headfile.h"

// PWMͨ��PIN����
#define DIR_2 P10 // ����
#define DIR_1 P24 // ����
#define PWM_2 PWMA_CH2N_P13
#define PWM_1 PWMA_CH4P_P26

// ������ŵ���İ�װ����棬��ʱ�뷽����תΪ��ת�ķ���
// ��DIR����Ϊ�߼���ʱ�������ת����DR����Ϊ�߼���ʱ�������ת��

extern int16 motor_left_pwm, motor_right_pwm;
extern int16 motor_left_pwm_last, motor_right_pwm_last;
void Motor_PWM_Init(void);
void Motor_PWM_Write(void);

#endif