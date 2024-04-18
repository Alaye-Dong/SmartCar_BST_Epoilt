/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2023,Alaye_Dong
 * All rights reserved
 *
 * �����������ݰ�Ȩ����Alaye_Dong���У�δ������������ ��ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣��Alaye_Dong�İ�Ȩ������
 *
 * @file       		menu
 * @author     		Alaye_Dong(QQ2971731849)
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @date       		2023-7-28
 * @note            Ϊ�۲���Ļ�����޸���_SEEKFREE_FONT_h��WHITE�ĺ궨�壬ʵ��Ϊ��ɫ
 *********************************************************************************************************************/

#include "menu.h"

//���尴������
#define KeystrokeONE 1
#define KeystrokeTWO 2
#define KeystrokeTHREE 3
#define KeystrokeFOUR 4

#define RowsMax 7 //�������Ļ�Ͽ��ƶ������������
#define RowsMin 1 //�������Ļ�Ͽ��ƶ�������С����
#define CenterColumn 10*8   //������
#define EEPROM_MODE 1 //eeporm��д������Ϊ1

int Display_Codename = 0;    //��ʾҳ�����
uint8 KeystrokeLabel = 0;   //��ѹ״̬
uint8 CursorRow= 0; //��������к�
uint8 MenuNextFlag = 0; //�����ָ�˵������־λ
float change_unit = 0;  //�����޸ĵĵ�λֵ

//��Ҫ���޸ĵĲ���ʾ��
int Start_Flag = 0,garage_out_direction = 0;
float PID_P = 0.123, PID_D = 0.123;
int normal_speed = 0;

//���в˵�ҳ��Ĵ�������������У���ֹ�ɼ�ͷ�����к����������벻���ڵĲ˵�
int menu_have_sub[] =  {
                            0,
                            1, 11, 12,
                            2, 21 ,22, 23
                       };

//�˵���ͷ��ʶ                       
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
    //��ͷ��λ
    if (CursorRow < RowsMin)
    {
        CursorRow= RowsMin;
    }
    else if (CursorRow > RowsMax)
    {
        CursorRow= RowsMin;
    } 
    //��ͷ��ӡ
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

//�˵����¼���ת
void Menu_Next_Back()
{
    switch(MenuNextFlag)
    {
        case 0:
                break;
        
        case -1:    //������һ��
                Display_Codename = Display_Codename / 10;
                CursorRow = 0;
                lcd_clear(WHITE);
                break;
        case 1:     //������һ��
                if(have_sub_menu(Display_Codename*10 + CursorRow))
                {
                    Display_Codename = Display_Codename*10 + CursorRow;
                    CursorRow = 0;
                    lcd_clear(WHITE);
                }

                break;
    }
    
    MenuNextFlag = 0;   //�л���ҳ�棬��־λ��0
}

// ��鱾���Ƿ�����Ӳ˵�
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

//���Ͳ����޸�
void Keystroke_int(int* parameter, int change_unit_MIN)
{
    change_unit = change_unit_MIN;  //�޸ĵ�λֵ = ��С�޸ĵ�λֵ
    Keystroke_Scan();
    
    //ͨ���������뿪�����л��޸ĵ�λֵ
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
                Display_Codename /= 10; //������һҳ
                lcd_clear(WHITE);
                break;
        case KeystrokeFOUR :
                Display_Codename = 0;	//���ص�0ҳ
                lcd_clear(WHITE);
                break;
    }

}

//�����Ͳ����޸�
void Keystroke_float(float* parameter, float change_unit_MIN)
{
    change_unit = change_unit_MIN;  //�޸ĵ�λֵ = ��С�޸ĵ�λֵ
    Keystroke_Scan();
    
    //ͨ���������뿪�����л��޸ĵĵ�λֵ
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
                Display_Codename /= 10; //������һҳ
                lcd_clear(WHITE);
                break;
        case KeystrokeFOUR :
                Display_Codename = 0;	//���ص�0ҳ
                lcd_clear(WHITE);
                break;
    }

}


//������ֵ�޸ģ�-1��1
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
                Display_Codename /= 10; //������һҳ
                lcd_clear(WHITE);
                break;
        case KeystrokeFOUR :
                Display_Codename = 0;	//���ص�0ҳ
                lcd_clear(WHITE);
                break;
    }
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      �˵�Ŀ¼
//  @param
//  @return     void
//  @note       ����while����ʾĿ��ҳ��  ��ÿ��ҳ�水�������󶼻�ı䵽��Ӧҳ�棬�˺������ڸ�����Ļ��ʾ        
//             ��ɾҳ��ͬʱ��ǵ�ͬ���޸�menu_have_sub[]�����ֵ
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
//  @brief      ���˵�Ŀ¼
//  @param
//  @return     void
//  @note       ��ҳ��Ϊ���Ϊ0
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
    
    if(MenuNextFlag == 1 && have_sub_menu(CursorRow))   //������һ���˵�
    {
        Display_Codename = CursorRow;
        CursorRow = 0;
        lcd_clear(WHITE);
    }
    
    else if(MenuNextFlag == -1 && EEPROM_MODE == 1) //�����˵�ʱ���»��˼�������4��������eepromȷ��ˢд
    {
        eeprom_flash();
        
        //ˢд�����ʾ
        lcd_clear(WHITE);
        lcd_showstr(1*8,1,"EEPROM_SAVED");
        lcd_showstr(1*8,4,"@author Alaye_Dong");    //���˾ͱ�ɾ.doge������

        delay_ms(400);
        BEEP = 1;
        delay_ms(100);
        BEEP = 0;
        lcd_clear(WHITE);
    }
    
    MenuNextFlag = 0;   //�л���ҳ�棬��־λ��0
}

/*///////////////////////////////////////
   ��ҳ��
*////////////////////////////////////////
void Menu_ONE_Display(uint8 control_line)
{
    lcd_showstr(CenterColumn - 2*8,0,"STRAT");
    lcd_showstr(1*8,1,"Start_Flag");
    lcd_showstr(1*8,2,"Out_Direction");

    lcd_showint32(14*8,1,Start_Flag,3);
    lcd_showint32(14*8,2,garage_out_direction,3);

    lcd_showstr(0,control_line,"&"); //&��־��ʾ
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

    lcd_showstr(0,control_line,"&"); //&��־��ʾ
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

