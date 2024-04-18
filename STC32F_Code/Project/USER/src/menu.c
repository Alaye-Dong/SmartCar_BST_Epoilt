/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2023,Alaye_Dong
 * All rights reserved
 *
 * 以下所有内容版权均属Alaye_Dong所有，未经允许不得用于 商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留Alaye_Dong的版权声明。
 *
 * @file       		menu
 * @author     		Alaye_Dong(QQ2971731849)
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @date       		2023-7-28
 * @note            为观察屏幕舒适修改了_SEEKFREE_FONT_h中WHITE的宏定义，实际为黑色
 *********************************************************************************************************************/

#include "menu.h"

//定义按键代号
#define KeystrokeONE 1
#define KeystrokeTWO 2
#define KeystrokeTHREE 3
#define KeystrokeFOUR 4

#define RowsMax 7 //光标在屏幕上可移动至的最大行数
#define RowsMin 1 //光标在屏幕上可移动至的最小行数
#define CenterColumn 10*8   //中央列
#define EEPROM_MODE 1 //eeporm读写开启则为1

int Display_Codename = 0;    //显示页面代号
uint8 KeystrokeLabel = 0;   //按压状态
uint8 CursorRow= 0; //光标所在行号
uint8 MenuNextFlag = 0; //光标所指菜单进入标志位
float change_unit = 0;  //单次修改的单位值

//需要被修改的参数示例
int Start_Flag = 0,garage_out_direction = 0;
float PID_P = 0.123, PID_D = 0.123;
int normal_speed = 0;

//将有菜单页面的代号填入该数组中，防止由箭头所在行号所决定进入不存在的菜单
int menu_have_sub[] =  {
                            0,
                            1, 11, 12,
                            2, 21 ,22, 23
                       };

//菜单箭头标识                       
void Cursor(void)
{		
    uint8 i;
    MenuNextFlag = 0;

    Keystroke_Scan();

    switch(KeystrokeLabel)
    {
        case 1:
                CursorRow--;
                break;                   
        case 2:
                CursorRow++;
                break;                    
        case 3:
                MenuNextFlag = 1;
                break;
        case 4:
                MenuNextFlag = -1;
                break;                    
    }
    //箭头限位
    if (CursorRow < RowsMin)
    {
        CursorRow= RowsMin;
    }
    else if (CursorRow > RowsMax)
    {
        CursorRow= RowsMin;
    } 
    //箭头打印
    for (i = 0; i <= 7; i++)
    {
        if(i == CursorRow)
        {
            lcd_showstr(0, CursorRow, ">");
        }
        else
        {
            lcd_showstr(0, i, " ");
        }
    }

}

//菜单上下级跳转
void Menu_Next_Back()
{
    switch(MenuNextFlag)
    {
        case 0:
                break;
        
        case -1:    //返回上一级
                Display_Codename = Display_Codename / 10;
                CursorRow = 0;
                lcd_clear(WHITE);
                break;
        case 1:     //进入下一级
                if(have_sub_menu(Display_Codename*10 + CursorRow))
                {
                    Display_Codename = Display_Codename*10 + CursorRow;
                    CursorRow = 0;
                    lcd_clear(WHITE);
                }

                break;
    }
    
    MenuNextFlag = 0;   //切换完页面，标志位归0
}

// 检查本行是否存在子菜单
int have_sub_menu(int menu_id) 
{   
    int i = 0;
    int len = sizeof(menu_have_sub) / sizeof(menu_have_sub [0]);
    for (i = 0; i <= len; i++)
    {
        if (menu_have_sub[i] == menu_id)
        {
            return 1;
        }
    }
    return 0;
}

//整型参数修改
void Keystroke_int(int* parameter, int change_unit_MIN)
{
    change_unit = change_unit_MIN;  //修改单位值 = 最小修改单位值
    Keystroke_Scan();
    
    //通过两个拨码开关来切换修改单位值
    if (sw1_status == 0)
    {
        change_unit = change_unit_MIN * 10; 
        if (sw2_status == 0)
        {
            change_unit = change_unit_MIN * 100; 
        }        
    }
    
    switch(KeystrokeLabel)
    {
        case 0 :
                break;
                    
        case KeystrokeONE :
                *parameter += change_unit;	
                break;
        case KeystrokeTWO :
                *parameter -= change_unit;			
                break;
                    
        case KeystrokeTHREE :
                Display_Codename /= 10; //返回上一页
                lcd_clear(WHITE);
                break;
        case KeystrokeFOUR :
                Display_Codename = 0;	//返回第0页
                lcd_clear(WHITE);
                break;
    }

}

//浮点型参数修改
void Keystroke_float(float* parameter, float change_unit_MIN)
{
    change_unit = change_unit_MIN;  //修改单位值 = 最小修改单位值
    Keystroke_Scan();
    
    //通过两个拨码开关来切换修改的单位值
    if (sw1_status == 0)
    {
        change_unit = change_unit_MIN * 10; 
        if (sw2_status == 0)
        {
            change_unit = change_unit_MIN * 100; 
        }        
    }
    
    switch(KeystrokeLabel)
    {
        case 0 :
                break;
                    
        case KeystrokeONE :
                *parameter += change_unit;	
                break;
        case KeystrokeTWO :
                *parameter -= change_unit;			
                break;
                    
        case KeystrokeTHREE :
                Display_Codename /= 10; //返回上一页
                lcd_clear(WHITE);
                break;
        case KeystrokeFOUR :
                Display_Codename = 0;	//返回第0页
                lcd_clear(WHITE);
                break;
    }

}


//整型特值修改，-1或1
void Keystroke_Special_Value(int* parameter)
{
    Keystroke_Scan();
    switch(KeystrokeLabel)
    {                
        case KeystrokeONE :
                *parameter = -1;	
                break;
        case KeystrokeTWO :
                *parameter = 1;	
                break;
                
        case KeystrokeTHREE :
                Display_Codename /= 10; //返回上一页
                lcd_clear(WHITE);
                break;
        case KeystrokeFOUR :
                Display_Codename = 0;	//返回第0页
                lcd_clear(WHITE);
                break;
    }
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      菜单目录
//  @param
//  @return     void
//  @note       启用while来显示目标页面  在每个页面按键按键后都会改变到对应页面，此函数用于更新屏幕显示        
//             增删页的同时请记得同步修改menu_have_sub[]数组的值
//-------------------------------------------------------------------------------------------------------------------
void Keystroke_Menu(void)
{
    switch(Display_Codename)
    {
        case 0:
            Keystroke_Menu_HOME();
            break;
            
        case 1:
            Keystroke_Menu_ONE(); 
            break;
        case 11:
            Keystroke_Menu_ONE_One();
            break;
        case 12:
            Keystroke_Menu_ONE_Two();
            break;
            
        case 2:
            Keystroke_Menu_TWO();
            break;
        case 21:
            Keystroke_Menu_TWO_One();
            break;
        case 22:
            Keystroke_Menu_TWO_Two();
            break;
        case 23:
            Keystroke_Menu_TWO_Three();
            break;

        default :
            break;
    
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      主菜单目录
//  @param
//  @return     void
//  @note       此页面为编号为0
//-------------------------------------------------------------------------------------------------------------------
void Keystroke_Menu_HOME(void)    //0
{
    while(MenuNextFlag == 0)
    {
        lcd_showstr(CenterColumn - 2*8,0,"MENU");
        lcd_showstr(1*8,1,"STRAT");
        lcd_showstr(1*8,2,"PID_SPEED");
        
        Cursor(); 
    }
    
    if(MenuNextFlag == 1 && have_sub_menu(CursorRow))   //进入下一级菜单
    {
        Display_Codename = CursorRow;
        CursorRow = 0;
        lcd_clear(WHITE);
    }
    
    else if(MenuNextFlag == -1 && EEPROM_MODE == 1) //在主菜单时按下回退键（按键4）来进行eeprom确认刷写
    {
        eeprom_flash();
        
        //刷写完成提示
        lcd_clear(WHITE);
        lcd_showstr(1*8,1,"EEPROM_SAVED");
        lcd_showstr(1*8,4,"@author Alaye_Dong");    //用了就别删.doge！！！

        delay_ms(400);
        BEEP = 1;
        delay_ms(100);
        BEEP = 0;
        lcd_clear(WHITE);
    }
    
    MenuNextFlag = 0;   //切换完页面，标志位归0
}

/*///////////////////////////////////////
   子页面
*////////////////////////////////////////
void Menu_ONE_Display(uint8 control_line)
{
    lcd_showstr(CenterColumn - 2*8,0,"STRAT");
    lcd_showstr(1*8,1,"Start_Flag");
    lcd_showstr(1*8,2,"Out_Direction");

    lcd_showint32(14*8,1,Start_Flag,3);
    lcd_showint32(14*8,2,garage_out_direction,3);

    lcd_showstr(0,control_line,"&"); //&标志提示
}

void Keystroke_Menu_ONE(void) //1
{	
    while(MenuNextFlag == 0)
    {
        Menu_ONE_Display(-1);
        Cursor();
    }
    Menu_Next_Back();
}

void Keystroke_Menu_ONE_One(void)  //11
{		
    Menu_ONE_Display(1);
    Keystroke_Special_Value(&Start_Flag);
}

void Keystroke_Menu_ONE_Two(void)  //12
{		
    Menu_ONE_Display(2);
    Keystroke_Special_Value(&garage_out_direction);
}

/*////////////////////////////////////
    PD
*////////////////////////////////////
void Menu_TWO_Display(uint8 control_line)
{
    lcd_showstr(CenterColumn - 4*8,0,"PID_SPEED");
    lcd_showstr(1*8,1,"P");
    lcd_showstr(1*8,2,"D");
    lcd_showstr(1*8,3,"normal_speed");

    lcd_showfloat(14*8,1,PID_P,2,3);
    lcd_showfloat(14*8,2,PID_D,2,3);
    lcd_showint32(14*8,3,normal_speed,3);

    lcd_showstr(0,control_line,"&"); //&标志提示
}

void Keystroke_Menu_TWO(void) //2
{	
    while(MenuNextFlag == 0)
    {
        Menu_TWO_Display(-1);
        Cursor();
    }
    Menu_Next_Back();
}

void Keystroke_Menu_TWO_One(void) //21
{
    Menu_TWO_Display(1);
    Keystroke_float(&PID_P, 0.001);
}
void Keystroke_Menu_TWO_Two(void)  //22
{
    Menu_TWO_Display(2);
    Keystroke_float(&PID_D, 0.001);
}
void Keystroke_Menu_TWO_Three(void)  //23
{
    Menu_TWO_Display(3);
    Keystroke_int(&normal_speed,1);
}

