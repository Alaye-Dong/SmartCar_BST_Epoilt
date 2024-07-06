///*********************************************************************************************************************
// * COPYRIGHT NOTICE
// * Copyright (c) 2020,��ɿƼ�
// * All rights reserved.
// * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
// *
// * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
// * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
// *
// * @file       		isr
// * @company	   		�ɶ���ɿƼ����޹�˾
// * @author     		��ɿƼ�(QQ790875685)
// * @version    		�鿴doc��version�ļ� �汾˵��
// * @Software 			MDK FOR C251 V5.60
// * @Target core		STC32F12K
// * @Taobao   			https://seekfree.taobao.com/
// * @date       		2020-4-14
// ********************************************************************************************************************/
#include "headfile.h"

// UART1�ж�
void UART1_Isr() INTERRUPT(4)
{
    uint8 res;
    static uint8 dwon_count;
    if (UART1_GET_TX_FLAG)
    {
        UART1_CLEAR_TX_FLAG;
        busy[1] = 0;
    }
    if (UART1_GET_RX_FLAG)
    {
        UART1_CLEAR_RX_FLAG;
        res = SBUF;
        // �����Զ�����
        if (res == 0x7F)
        {
            if (dwon_count++ > 20)
                IAP_CONTR = 0x60;
        }
        else
        {
            dwon_count = 0;
        }
    }
}

// UART2�ж�
void UART2_Isr() INTERRUPT(8)
{
    if (UART2_GET_TX_FLAG)
    {
        UART2_CLEAR_TX_FLAG;
        busy[2] = 0;
    }
    if (UART2_GET_RX_FLAG)
    {
        UART2_CLEAR_RX_FLAG;
        // �������ݼĴ���Ϊ��S2BUF
        if (wireless_type == WIRELESS_SI24R1)
        {
            wireless_uart_callback(); // ����ת���ڻص�����
        }
        // else if (wireless_type == WIRELESS_CH9141)
        // {
        //     bluetooth_ch9141_uart_callback(); // ����ת���ڻص�����
        // }
    }
}

// UART3�ж�
void UART3_Isr() INTERRUPT(17)
{
    if (UART3_GET_TX_FLAG)
    {
        UART3_CLEAR_TX_FLAG;
        busy[3] = 0;
    }
    if (UART3_GET_RX_FLAG)
    {
        UART3_CLEAR_RX_FLAG;
        // �������ݼĴ���Ϊ��S3BUF
    }
}

// UART4�ж�
void UART4_Isr() INTERRUPT(18)
{
    if (UART4_GET_TX_FLAG)
    {
        UART4_CLEAR_TX_FLAG;
        busy[4] = 0;
    }
    if (UART4_GET_RX_FLAG)
    {
        UART4_CLEAR_RX_FLAG;
        // �������ݼĴ���Ϊ��S4BUF;
    }
}

#define LED P52
void INT0_Isr() INTERRUPT(0)
{
    LED = 0; // ����LED
}

void INT1_Isr() INTERRUPT(2)
{
}
void INT2_Isr() INTERRUPT(10)
{
    INT2_CLEAR_FLAG; // ����жϱ�־
}

void INT3_Isr() INTERRUPT(11)
{
    INT3_CLEAR_FLAG; // ����жϱ�־
}

void INT4_Isr() INTERRUPT(16)
{
    INT4_CLEAR_FLAG; // ����жϱ�־
}

void TM0_Isr() INTERRUPT(1)
{
}

void TM1_Isr() INTERRUPT(3)
{
}

void TM2_Isr() INTERRUPT(12)
{
    TIM2_CLEAR_FLAG; // ����жϱ�־
}

void TM3_Isr() INTERRUPT(19)
{
    TIM3_CLEAR_FLAG; // ����жϱ�־
}

extern void pit_callback(void);

void TM4_Isr() INTERRUPT(20)
{
    TIM4_CLEAR_FLAG; // ����жϱ�־

    Inductor_Process(); // ���
    IMU_Process();
    Encoder_Process();
    Position_Analyse();

    // Ԫ��ʱ���Ը��ǵ�����õ�position
    // if (right_angle_flag != RIGHT_ANGLE_NONE)
    // {
    //     Right_Angle_Turn_Process();
    // }
    // else if (round_flag != ROUND_STATE_NONE)
    if (round_flag != ROUND_STATE_NONE)
    {
        Round_Turn_Process();
    }
    else if (obstacle_flag != OBSTACLE_NONE)
    {
        Obstacle_Turn_Process();
    }
    // else if (cross_flag != CROSS_NONE)
    // {
    //     Cross_Turn_Process();
    // }

    PID_Process();
    Motor_PWM_Write();

    print_send_flag = 1;
}

// void  INT0_Isr()  INTERRUPT(0);
// void  TM0_Isr()   INTERRUPT(1);
// void  INT1_Isr()  INTERRUPT(2);
// void  TM1_Isr()   INTERRUPT(3);
// void  UART1_Isr() INTERRUPT(4);
// void  ADC_Isr()   INTERRUPT(5);
// void  LVD_Isr()   INTERRUPT(6);
// void  PCA_Isr()   INTERRUPT(7);
// void  UART2_Isr() INTERRUPT(8);
// void  SPI_Isr()   INTERRUPT(9);
// void  INT2_Isr()  INTERRUPT(10);
// void  INT3_Isr()  INTERRUPT(11);
// void  TM2_Isr()   INTERRUPT(12);
// void  INT4_Isr()  INTERRUPT(16);
// void  UART3_Isr() INTERRUPT(17);
// void  UART4_Isr() INTERRUPT(18);
// void  TM3_Isr()   INTERRUPT(19);
// void  TM4_Isr()   INTERRUPT(20);
// void  CMP_Isr()   INTERRUPT(21);
// void  I2C_Isr()   INTERRUPT(24);
// void  USB_Isr()   INTERRUPT(25);
// void  PWM1_Isr()  INTERRUPT(26);
// void  PWM2_Isr()  INTERRUPT(27);