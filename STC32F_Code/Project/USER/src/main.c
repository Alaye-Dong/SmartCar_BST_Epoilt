/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2023-07-27

 ********************************************************************************************************************/
#include "headfile.h"

// 关于内核频率的设定，可以查看board.h文件
// 在board_init中,已经将P54引脚设置为复位
// 如果需要使用P54引脚,可以在board.c文件中的board_init()函数中删除SET_P54_RESRT即可

void main()
{
    clock_init(SYSTEM_CLOCK_52M); // 初始化系统频率,勿删除此句代码。
    board_init();                 // 初始化寄存器,勿删除此句代码。

    // 此处编写用户代码 例如外设初始化代码等
    lcd_init(); // 屏幕初始化
    display_codename = 3;
    // !使用到了未实际连接的P24引脚作spi初始化，该脚与电机控制方向脚引脚冲突
    // 必须lcd_init()后再进行Motor_PWM_Init()中的gpio_mode(P2_4, GPO_PP)， 否则会导致电机控制方向脚输出异常
    // 逐飞在STC32F开源库 V1.2.6(2024-05-07) 后似乎已经修复了这个bug

    Wireless_Debug_Init(); // 初始化无线转串口

    Magnet_ADC_Init(); // 电磁ADC初始化

    IMU_Init(); // 姿态传感器初始化

    Motor_PWM_Init(); // PWM初始化

    Encoder_Init(); // 编码器初始化

    BEEP_Init(); // 蜂鸣器初始化

    ToF_Init(); // ToF DL1B初始化

    eeprom_init(); // eeprom初始化
    PID_Init();

    pit_timer_ms(TIM_4, 5); // 设置中断定时

    Beep_Buzzing(100, 3); // 开机提示蜂鸣

    while (1)
    {
        // 此处编写需要循环执行的代码
        Wireless_Seekfree_Assistant_Debug();
        Debug_Parameter_Oscilloscope_Send();

        // ! 屏幕菜单极耗性能，开启后串口刷新将只有大概5fps，不开启则为250fps
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
