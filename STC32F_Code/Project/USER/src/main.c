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
    lcd_init(); // ��Ļ��ʼ��
    display_codename = 3;
    // !ʹ�õ���δʵ�����ӵ�P24������spi��ʼ�����ý��������Ʒ�������ų�ͻ
    // ����lcd_init()���ٽ���Motor_PWM_Init()�е�gpio_mode(P2_4, GPO_PP)�� ����ᵼ�µ�����Ʒ��������쳣
    // �����STC32F��Դ�� V1.2.6(2024-05-07) ���ƺ��Ѿ��޸������bug

    Wireless_Debug_Init(); // ��ʼ������ת����

    Magnet_ADC_Init(); // ���ADC��ʼ��

    IMU_Init(); // ��̬��������ʼ��

    Motor_PWM_Init(); // PWM��ʼ��

    Encoder_Init(); // ��������ʼ��

    BEEP_Init(); // ��������ʼ��

    ToF_Init(); // ToF DL1B��ʼ��

    eeprom_init(); // eeprom��ʼ��
    PID_Init();

    pit_timer_ms(TIM_4, 5); // �����ж϶�ʱ

    Beep_Buzzing(100, 3); // ������ʾ����

    while (1)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���
        Wireless_Seekfree_Assistant_Debug();
        Debug_Parameter_Oscilloscope_Send();

        // ! ��Ļ�˵��������ܣ������󴮿�ˢ�½�ֻ�д��5fps����������Ϊ250fps
        Keystroke_Menu();

        if (element_busy_flag != 1)
        {
            Right_Angle_Recognition();
            Round_Recognition();
            Obstacle_Recognition();
            // Cross_Recognition();
        }
    }
}
