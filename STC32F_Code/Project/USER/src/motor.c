#include "motor.h"

// PWMͨ������
#define PWM_1 PWMA_CH1P_P10 // ����
#define PWM_2 PWMA_CH2N_P13 // ����

// �������,=1����ת
#define PWM_1_DIR P24 // ����
#define PWM_2_DIR P26 // ����

void Motor_PWM_Init(void)
{
    // PWM���ų�ʼ��
    pwm_init(PWM_1, 17000, 0);
    pwm_init(PWM_2, 17000, 0);
    // ��������źų�ʼ��
    gpio_mode(PWM_1_DIR, GPO_PP);
    gpio_mode(PWM_2_DIR, GPO_PP);
}