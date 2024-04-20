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
	clock_init(SYSTEM_CLOCK_52M);	// ��ʼ��ϵͳƵ��,��ɾ���˾���롣
	board_init();					// ��ʼ���Ĵ���,��ɾ���˾���롣

    // �˴���д�û����� ���������ʼ�������
  
    //ADC��ʼ��
    // adc_init(ADC_P00,ADC_SYSclk_DIV_2);    
    // adc_init(ADC_P01,ADC_SYSclk_DIV_2);
    // adc_init(ADC_P05,ADC_SYSclk_DIV_2);
    // adc_init(ADC_P06,ADC_SYSclk_DIV_2);
    // adc_init(ADC_P11,ADC_SYSclk_DIV_2);
    // adc_init(ADC_P13,ADC_SYSclk_DIV_2);
    
    //PWM���ų�ʼ��
    // pwm_init(PWM1, 17000, 0);
    // pwm_init(PWM2, 17000, 0);

    //��Ļ��ʼ��
    lcd_init();
    
    //��������źų�ʼ��
    // gpio_mode(P6_4, GPO_PP);    
    // gpio_mode(P6_0, GPO_PP);    
    
    //pwm_init(PWMB_CH1_P74, 50, 692);//��� ���765   ��С625

    //��������������ʼ��
    ctimer_count_init(CTIM0_P34);   
    ctimer_count_init(CTIM3_P04);

	//���������ǳ�ʼ��			  
    // while(imu660ra_init())				
    // {
    //     delay_ms(500);
    //     printf("imu660ra init try again.\r\n");
    // }

    //dl1a_init();    //TOF DL1A ��ʼ��
	eeprom_init();	//eeprom��ʼ��
    
    pit_timer_ms(TIM_4, 5);     //�����ж϶�ʱ
	gpio_mode(P3_2, GPO_PP);    //������
    BEEP = 1;
    delay_ms(10);
    BEEP = 0;
	
    while(1)
    {
		// �˴���д��Ҫѭ��ִ�еĴ���
        
        // �˴���д��Ҫѭ��ִ�еĴ���
        Keystroke_Scan();
        Keystroke_Menu();
    }
}


