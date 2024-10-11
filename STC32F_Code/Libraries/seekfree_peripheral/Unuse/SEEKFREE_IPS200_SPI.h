/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		IPS200_SPI
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32F12K
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-11-15
 * @note		
					接线定义：
					------------------------------------ 
					2.0寸IPS模块管脚       	单片机管脚
					SCL                 	查看SEEKFREE_IPS200_SPI.h文件内的IPS200_SCL		宏定义     硬件SPI引脚不可随意切换
					SDA                 	查看SEEKFREE_IPS200_SPI.h文件内的IPS200_SDA		宏定义     硬件SPI引脚不可随意切换
					RES                 	查看SEEKFREE_IPS200_SPI.h文件内的IPS200_REST_PIN 宏定义    
					DC                  	查看SEEKFREE_IPS200_SPI.h文件内的IPS200_DC_PIN	宏定义  
					CS                  	查看SEEKFREE_IPS200_SPI.h文件内的IPS200_CS		宏定义     硬件SPI引脚不可随意切换
					BL  					查看SEEKFREE_IPS200_SPI.h文件内的IPS200_BL_PIN	宏定义
					
					电源引脚
					VCC 3.3V电源
					GND 电源地
					最大分辨率320*240
					------------------------------------ 
 ********************************************************************************************************************/
#ifndef _SEEKRE_IPS200_h_
#define _SEEKRE_IPS200_h_

#include "common.h"
#include "board.h"


#define IPS200_SPIN_PIN     SPI_CH2           //定义使用的SPI号
#define	IPS200_SCL_PIN 		SPI_CH2_SCLK_P25	//定义SPI_SCK引脚
#define	IPS200_SDA_PIN		SPI_CH2_MOSI_P23   //定义SPI_MOSI引脚
#define	IPS200_SDA_IN_PIN	SPI_NULL_PIN   //定义SPI_MISO引脚  IPS屏幕没有MISO引脚，但是这里任然需要定义，在spi的初始化时需要使用
#define IPS200_RST_PIN  	P20
#define IPS200_DC_PIN 	 	P21   	//液晶命令位引脚定义
#define IPS200_CS_PIN    	P22   	//定义SPI_CS引脚
#define IPS200_BL_PIN    	P27     //液晶背光引脚定义 


#define IPS200_RST(x)      	(IPS200_RST_PIN = x)
#define IPS200_DC(x)        (IPS200_DC_PIN = x)
#define IPS200_CS(x)        (IPS200_CS_PIN = x)
#define IPS200_BL(x)        (IPS200_BL_PIN = x)


#define IPS200_DEFAULT_DISPLAY_DIR      (IPS200_PORTAIT)                        // 默认的显示方向
#define IPS200_DEFAULT_PENCOLOR         (RED    )                        // 默认的画笔颜色
#define IPS200_DEFAULT_BGCOLOR          (WHITE  )                        // 默认的背景颜色
#define IPS200_DEFAULT_DISPLAY_FONT     (IPS200_8X16_FONT)                      // 默认的字体模式


typedef enum
{
    IPS200_TYPE_SPI,                                                            // SPI 驱动
    IPS200_TYPE_PARALLEL8,                                                      // 并口驱动
}ips200_type_enum;

typedef enum
{
    IPS200_PORTAIT                      = 0,                                    // 竖屏模式
    IPS200_PORTAIT_180                  = 1,                                    // 竖屏模式  旋转180
    IPS200_CROSSWISE                    = 2,                                    // 横屏模式
    IPS200_CROSSWISE_180                = 3,                                    // 横屏模式  旋转180
}ips200_dir_enum;

typedef enum
{
    IPS200_6X8_FONT                     = 0,                                    // 6x8      字体
    IPS200_8X16_FONT                    = 1,                                    // 8x16     字体
    IPS200_16X16_FONT                   = 2,                                    // 16x16    字体 目前不支持
}ips200_font_size_enum;

void    ips200_clear                    (void);
void    ips200_full                     (const uint16 color);
void    ips200_set_dir                  (ips200_dir_enum dir);
void    ips200_set_font                 (ips200_font_size_enum font);
void    ips200_set_color                (const uint16 pen, const uint16 bgcolor);
void    ips200_draw_point               (uint16 x, uint16 y, const uint16 color);
void    ips200_draw_line                (uint16 x_start, uint16 y_start, uint16 x_end, uint16 y_end, const uint16 color);

void    ips200_show_char                (uint16 x, uint16 y, const char dat);
void    ips200_show_string              (uint16 x, uint16 y, const char dat[]);
void    ips200_show_int                 (uint16 x, uint16 y, const int32 dat, uint8 num);
void    ips200_show_uint                (uint16 x, uint16 y, const uint32 dat, uint8 num);
void    ips200_show_float               (uint16 x, uint16 y, const double dat, uint8 num, uint8 pointnum);


void    ips200_show_wave                (uint16 x, uint16 y, const uint16 *wave, uint16 width, uint16 value_max, uint16 dis_width, uint16 dis_value_max);
void    ips200_show_chinese             (uint16 x, uint16 y, uint8 size, const uint8 *chinese_buffer, uint8 number, const uint16 color);



void    ips200_init_spi                     ();

#endif
