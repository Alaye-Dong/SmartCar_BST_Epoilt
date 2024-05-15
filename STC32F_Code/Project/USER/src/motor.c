#include "motor.h"

// PWM通道PIN定义
#define PWM_1 PWMA_CH1P_P10 // 左轮
#define PWM_2 PWMA_CH2N_P13 // 右轮

// 电机方向PIN,=1则正转
#define PWM_1_DIR P24 // 左轮
#define PWM_2_DIR P26 // 右轮

#define MORTOR_PWM_MIN -3500
#define MORTOR_PWM_MAX 7000

int16 motor_left_pwm = 0;
int16 motor_right_pwm = 0;

void Motor_PWM_Init(void)
{
    // PWM引脚初始化
    pwm_init(PWM_1, 17000, 0);
    pwm_init(PWM_2, 17000, 0);
    // 电机方向信号初始化
    gpio_mode(PWM_1_DIR, GPO_PP);
    gpio_mode(PWM_2_DIR, GPO_PP);
}

//限幅PWM值后，输出PWM到电机
void Motor_PWM_Write(void)
{
    motor_left_pwm = FUNC_LIMIT_AB(motor_left_pwm, MORTOR_PWM_MIN, MORTOR_PWM_MAX); // 电机PWM限幅
    motor_right_pwm = FUNC_LIMIT_AB(motor_right_pwm, MORTOR_PWM_MIN, MORTOR_PWM_MAX);

    if (motor_left_pwm > 0)
    {
        PWM_1_DIR = 1;
        pwm_duty(PWM_1, motor_left_pwm);
    }
    else
    {
        PWM_1_DIR = 0;
        pwm_duty(PWM_1, abs(motor_left_pwm));
    }

    if (motor_right_pwm > 0)
    {
        PWM_2_DIR = 0;
        pwm_duty(PWM_2, motor_right_pwm);
    }
    else
    {
        PWM_2_DIR = 1;
        pwm_duty(PWM_2, abs(motor_right_pwm));
    }
}

