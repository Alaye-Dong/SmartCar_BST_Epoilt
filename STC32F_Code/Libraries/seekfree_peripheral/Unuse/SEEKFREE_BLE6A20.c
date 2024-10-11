/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		逐飞科技ble6a20蓝牙模块
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


#include "SEEKFREE_BLE6A20.h"
#include "zf_uart.h"
#include "zf_fifo.h"


static  fifo_struct     ble6a20_fifo;
static  uint8           ble6a20_buffer[BLE6A20_BUFFER_SIZE];  // 数据存放数组

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ble6a20蓝牙模块回调函数
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       
//-------------------------------------------------------------------------------------------------------------------
void ble6a20_callback(uint8 dat)
{
    // 接到一个字节后单片机将会进入串口中断，通过在此处读取dat可以取走数据
    fifo_write_buffer(&ble6a20_fifo, &dat, 1);       // 存入 FIFO
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ble6a20蓝牙模块发送一个字节
//  @param      dat 	需要发送的字节长度
//  @return     uint8	0成功，1失败						
//  @since      v1.0
//  Sample usage:	
//  @note       
//-------------------------------------------------------------------------------------------------------------------
uint8 ble6a20_send_byte(uint8 dat)
{
	if(BLE6A20_RTS_PIN == 1)  
	{
		return 1;//模块忙,如果允许当前程序使用while等待 则可以使用后面注释的while等待语句替换本if语句
	}
	
	uart_putchar(BLE6A20_UART_INDEX, dat);
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ble6a20蓝牙模块 发送函数
//  @param      buff        需要发送的数据地址
//  @param      len         发送长度
//  @return     uint32      剩余未发送的字节数   
//  @since      v1.0
//  Sample usage:	
//  @note       
//-------------------------------------------------------------------------------------------------------------------
uint32 ble6a20_send_buff(uint8 *buff, uint32 len)
{
    while(len>30)
    {
        if(BLE6A20_RTS_PIN == 1)  
        {
            return len;//模块忙,如果允许当前程序使用while等待 则可以使用后面注释的while等待语句替换本if语句
        }
        //while(RTS_PIN);  //如果RTS为低电平，则继续发送数据
        uart_putbuff(BLE6A20_UART_INDEX,buff,30);

        buff += 30; //地址偏移
        len -= 30;//数量
    }
    
    if(BLE6A20_RTS_PIN == 1)  
    {
        return len;//模块忙,如果允许当前程序使用while等待 则可以使用后面注释的while等待语句替换本if语句
    }
    //while(WIRELESS_RTS_PIN);  //如果RTS为低电平，则继续发送数据
    uart_putbuff(BLE6A20_UART_INDEX,buff,len);//发送最后的数据
    
    return 0;
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      ble6a20蓝牙模块 读取函数
//  @param      buff            存储的数据地址
//  @param      len             长度
//  @return     uint32          实际读取字节数
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint32 ble6a20_read_buff (uint8 *buff, uint32 len)
{
    uint32 data_len = len;
    fifo_read_buffer(&ble6a20_fifo, buff, &data_len, FIFO_READ_AND_CLEAN);
    return data_len;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ble6a20蓝牙模块初始化
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       
//-------------------------------------------------------------------------------------------------------------------
void ble6a20_init(void)
{
    BLE6A20_RTS_PIN = 0;
    wireless_type = WIRELESS_BLE6A20;
	wireless_module_uart_handler = ble6a20_callback;
    //本函数使用的波特率为115200，为ble6a20蓝牙模块的默认波特率，如需其他波特率请自行配置模块并修改串口的波特率
    fifo_init(&ble6a20_fifo, FIFO_DATA_8BIT, ble6a20_buffer, BLE6A20_BUFFER_SIZE);
	uart_init(BLE6A20_UART_INDEX, BLE6A20_RX_PIN, BLE6A20_TX_PIN, BLE6A20_BAUD, BLE6A20_TIMER_N);	//初始化串口    
    
}