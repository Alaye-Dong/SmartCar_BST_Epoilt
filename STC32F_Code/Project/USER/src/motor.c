#include "motor.h"

#define MORTOR_PWM_MIN -3000
#define MORTOR_PWM_MAX 3000

int16 motor_left_pwm = 0;
int16 motor_right_pwm = 0;
void Motor_PWM_Init(void)
{
    // PWM引脚初始化
    pwm_init(PWM_1, 17000, 0);
    pwm_init(PWM_2, 17000, 0);

    // 电机方向信号初始化
    gpio_mode(P1_0, GPO_PP);
    gpio_mode(P2_4, GPO_PP);
}

// 限幅PWM值后，输出PWM到电机
void Motor_PWM_Write(void)
{
    motor_left_pwm = FUNC_LIMIT_AB(motor_left_pwm, MORTOR_PWM_MIN, MORTOR_PWM_MAX); // 电机PWM限幅
    motor_right_pwm = FUNC_LIMIT_AB(motor_right_pwm, MORTOR_PWM_MIN, MORTOR_PWM_MAX);

    if (motor_left_pwm >= 0) // 正转
    {
        DIR_1 = 1;
        pwm_duty(PWM_1, motor_left_pwm);
    }
    else // 反转
    {
        DIR_1 = 0;
        pwm_duty(PWM_1, -motor_left_pwm);
    }

    if (motor_right_pwm >= 0) // 正转
    {

        DIR_2 = 1;
        pwm_duty(PWM_2, motor_right_pwm);
    }
    else // 反转
    {

        DIR_2 = 0;
        pwm_duty(PWM_2, -motor_right_pwm);
    }
}
