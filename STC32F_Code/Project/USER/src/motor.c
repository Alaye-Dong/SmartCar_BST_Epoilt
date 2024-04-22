#include "motor.h"

// PWM通道定义
#define PWM_1 PWMA_CH1P_P10 // 左轮
#define PWM_2 PWMA_CH2N_P13 // 右轮

// 电机方向,=1则正转
#define PWM_1_DIR P24 // 左轮
#define PWM_2_DIR P26 // 右轮

void Motor_PWM_Init(void)
{
    // PWM引脚初始化
    pwm_init(PWM_1, 17000, 0);
    pwm_init(PWM_2, 17000, 0);
    // 电机方向信号初始化
    gpio_mode(PWM_1_DIR, GPO_PP);
    gpio_mode(PWM_2_DIR, GPO_PP);
}