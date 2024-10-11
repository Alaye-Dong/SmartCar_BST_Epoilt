/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		逐飞科技无线转串口模块
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-03-27
 * @note		
					接线定义：
					------------------------------------ 
					BLE6A20蓝牙      单片机                        
    				RX              查看SEEKFREE_BLE6A20.h文件中的BLE6A20_TX_PIN宏定义
    				TX              查看SEEKFREE_BLE6A20.h文件中的BLE6A20_RX_PIN宏定义
    				RTS             查看SEEKFREE_BLE6A20.h文件中的BLE6A20_RTS_PIN宏定义
					------------------------------------ 
 ********************************************************************************************************************/

#ifndef _SEEKFREE_BLE6A20_
#define _SEEKFREE_BLE6A20_

#include "common.h"


#define BLE6A20_UART_INDEX        		UART_2         //无线转串口模块 所使用到的串口     
#define BLE6A20_TX_PIN    				UART2_TX_P47
#define BLE6A20_RX_PIN    				UART2_RX_P46
#define BLE6A20_TIMER_N    				TIM_2
#define BLE6A20_BAUD   					115200
#define BLE6A20_RTS_PIN 				P45 			//定义流控位引脚  指示当前模块是否可以接受数据  0可以继续接收  1不可以继续接收

#define BLE6A20_BUFFER_SIZE       		64
#define BLE6A20_TIMEOUT_COUNT     		500


void ble6a20_init(void);
void ble6a20_callback(uint8 dat);

uint8  ble6a20_send_byte(uint8 dat);
uint32 ble6a20_send_buff(uint8 *buff, uint32 len);
uint32 ble6a20_read_buff(uint8 *buff, uint32 len);

#endif 
