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

    Wireless_Debug_Init();

    Magnet_ADC_Init(); // ���ADC��ʼ��

    Motor_PWM_Init(); // PWM��ʼ��

    Encoder_Init(); // ��������ʼ��

    BEEP_Init(); // ��������ʼ��

    eeprom_init(); // eeprom��ʼ��

    PID_Init();

    pit_timer_ms(TIM_4, 5); // �����ж϶�ʱ

    BEEP_ON_ms(100);

    while (1)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���
        Wireless_Debug_Main();
        Keystroke_Menu();

        //uint8 test_str[] = "\r\n seekfree.taobao.com. \r\n";
        // wireless_uart_send_buff(test_str, sizeof(test_str) - 1);
        // // ��ȡfifo�е�����
        // dat_len = wireless_uart_read_buff(read_buf, 10);
        // // �����ȡ������
        // if (dat_len != 0)
    }
}
