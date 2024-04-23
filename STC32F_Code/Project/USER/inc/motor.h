#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "headfile.h"

void Motor_PWM_Init(void);
void Motor_PWM_Write(void);
int Limit(int x, int min, int max);


#endif