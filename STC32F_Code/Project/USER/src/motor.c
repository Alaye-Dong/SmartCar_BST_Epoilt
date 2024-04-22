#include "motor.h"

// PWM通道PIN定义
#define PWM_1 PWMA_CH1P_P10 // 左轮
#define PWM_2 PWMA_CH2N_P13 // 右轮

// 电机方向PIN,=1则正转
#define PWM_1_DIR P24 // 左轮
#define PWM_2_DIR P26 // 右轮

#define MORTOR_PWM_MIN -3500
#define MORTOR_PWM_MAX 7000

int duty_PWM_1 = 0;
int duty_PWM_2 = 0;

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
    duty_PWM_1 = Limit(duty_PWM_1, MORTOR_PWM_MIN, MORTOR_PWM_MAX); // 电机PWM限幅
    duty_PWM_2 = Limit(duty_PWM_2, MORTOR_PWM_MIN, MORTOR_PWM_MAX);

    if (duty_PWM_1 > 0)
    {
        PWM_1_DIR = 1;
        pwm_duty(PWM_1, duty_PWM_1);
    }
    else
    {
        PWM_1_DIR = 0;
        pwm_duty(PWM_1, abs(duty_PWM_1));
    }

    if (duty_PWM_2 > 0)
    {
        PWM_2_DIR = 0;
        pwm_duty(PWM_2, duty_PWM_2);
    }
    else
    {
        PWM_2_DIR = 1;
        pwm_duty(PWM_2, abs(duty_PWM_2));
    }
}

int Limit(int x, int min, int max)
{
    if (x < min)
    {
        return min;
    }
    else if (x > max)
    {
        return max;
    }
    else
    {
        return x;
    }
}