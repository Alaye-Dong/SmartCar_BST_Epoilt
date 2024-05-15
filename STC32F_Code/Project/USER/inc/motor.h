#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "headfile.h"

extern int16 motor_left_pwm, motor_right_pwm;

void Motor_PWM_Init(void);
void Motor_PWM_Write(void);



#endif