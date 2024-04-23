/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ790875685)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2023-07-27

 ********************************************************************************************************************/
#include "headfile.h"

// �����ں�Ƶ�ʵ��趨�����Բ鿴board.h�ļ�
// ��board_init��,�Ѿ���P54��������Ϊ��λ
// �����Ҫʹ��P54����,������board.c�ļ��е�board_init()������ɾ��SET_P54_RESRT����

void main()
{
    clock_init(SYSTEM_CLOCK_52M); // ��ʼ��ϵͳƵ��,��ɾ���˾���롣
    board_init();                 // ��ʼ���Ĵ���,��ɾ���˾���롣

    // �˴���д�û����� ���������ʼ�������

    // ADC��ʼ��
    //  adc_init(ADC_P00,ADC_SYSclk_DIV_2);
    //  adc_init(ADC_P01,ADC_SYSclk_DIV_2);
    //  adc_init(ADC_P05,ADC_SYSclk_DIV_2);
    //  adc_init(ADC_P06,ADC_SYSclk_DIV_2);
    //  adc_init(ADC_P11,ADC_SYSclk_DIV_2);
    //  adc_init(ADC_P13,ADC_SYSclk_DIV_2);

    Motor_PWM_Init();

    // pwm_init(PWMB_CH1_P74, 50, 692);//��� ���765   ��С625

    Encoder_Init();

    // ���������ǳ�ʼ��
    //  while(imu660ra_init())
    //  {
    //      delay_ms(500);
    //      printf("imu660ra init try again.\r\n");
    //  }

    // dl1a_init();    //TOF DL1A ��ʼ��
    BEEP_Init();
    lcd_init();    // ��Ļ��ʼ��
    eeprom_init(); // eeprom��ʼ��

    pit_timer_ms(TIM_4, 5); // �����ж϶�ʱ

    BEEP_ON_ms(100);

    while (1)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���
        Keystroke_Menu();
    }
}
