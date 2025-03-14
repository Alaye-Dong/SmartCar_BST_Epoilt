///*********************************************************************************************************************
// * COPYRIGHT NOTICE
// * Copyright (c) 2020,逐飞科技
// * All rights reserved.
// * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
// *
// * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
// * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
// *
// * @file       		isr
// * @company	   		成都逐飞科技有限公司
// * @author     		逐飞科技(QQ790875685)
// * @version    		查看doc内version文件 版本说明
// * @Software 			MDK FOR C251 V5.60
// * @Target core		STC32F12K
// * @Taobao   			https://seekfree.taobao.com/
// * @date       		2020-4-14
// ********************************************************************************************************************/
#include "headfile.h"

// UART1中断
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
        // 程序自动下载
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

// UART2中断
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
        // 接收数据寄存器为：S2BUF
        if (wireless_module_uart_handler != NULL)
        {
            // 该函数为函数指针
            // 再初始化无线模块的时候，设置该函数地址
            wireless_module_uart_handler(S2BUF);
        }
    }
}

// UART3中断
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
        // 接收数据寄存器为：S3BUF
    }
}

// UART4中断
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
        // 接收数据寄存器为：S4BUF;
    }
}

#define LED P52
void INT0_Isr() INTERRUPT(0)
{
    LED = 0; // 点亮LED
}

void INT1_Isr() INTERRUPT(2)
{
}
void INT2_Isr() INTERRUPT(10)
{
    INT2_CLEAR_FLAG; // 清除中断标志
}

void INT3_Isr() INTERRUPT(11)
{
    INT3_CLEAR_FLAG; // 清除中断标志
}

void INT4_Isr() INTERRUPT(16)
{
    INT4_CLEAR_FLAG; // 清除中断标志
}

void TM0_Isr() INTERRUPT(1)
{
}

void TM1_Isr() INTERRUPT(3)
{
}

void TM2_Isr() INTERRUPT(12)
{
    TIM2_CLEAR_FLAG; // 清除中断标志
}

void TM3_Isr() INTERRUPT(19)
{
    TIM3_CLEAR_FLAG; // 清除中断标志
}

extern void pit_callback(void);

void TM4_Isr() INTERRUPT(20)
{
    TIM4_CLEAR_FLAG; // 清除中断标志

    Inductor_Process(); // 电感
    IMU_Process();
    Encoder_Process();
    Position_Analyse();

    // 元素时可以覆盖掉计算得的position
    switch (element_busy_flag)
    {
    case ELEMENT_NONE:
        break;
    case ELEMENT_OBSTACLE:
        Obstacle_Turn_Process();
        break;

    case ELEMENT_ROUND:
        Round_Turn_Process();
        break;

    case ELEMENT_RAMP:
        // TODO Ramp_Process();
        break;

    default:
        break;
    }

    PID_Process();
    if (start_flag == 1)
    {
        Motor_PWM_Write();
    }

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