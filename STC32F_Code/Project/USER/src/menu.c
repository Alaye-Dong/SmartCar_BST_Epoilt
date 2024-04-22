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

// ���尴������
#define KEYSTROKE_ONE 1
#define KEYSTROKE_TWO 2
#define KEYSTROKE_THREE 3
#define KEYSTROKE_FOUR 4

#define ROWS_MAX 7           // �������Ļ�Ͽ��ƶ������������
#define ROWS_MIN 1           // �������Ļ�Ͽ��ƶ�������С����
#define CENTER_COLUMN 10 * 8 // ������
#define EEPROM_MODE 1        // eeporm��д������Ϊ1

int display_codename = 0; // ��ʾҳ�����
uint8 cursor_row = 0;     // ��������к�
uint8 previous_cursor_row = -1;  // ��һ�ι�������к�
uint8 menu_next_flag = 0; // �����ָ�˵������־λ
float change_unit = 0;    // �����޸ĵĵ�λֵ
int change_unit_multiplier = 1; // �޸ĵ�λ����
int keystroke_three_count = 0; // ����һ��ȫ�ֱ�����¼KEYSTROKE_THREE�Ĵ�������

// ��Ҫ���޸ĵĲ���ʾ��
int start_flag = 0, garage_out_direction = 0;
float PID_P = 0.123, PID_D = 0.123;
int normal_speed = 0;

// ���в˵�ҳ��Ĵ�������������У���ֹ�ɼ�ͷ�����к����������벻���ڵĲ˵�
int menu_have_sub[] = {
    0,
    1, 11, 12,
    2, 21, 22, 23};

// �˵���ͷ��ʶ
void Cursor(void)
{
    menu_next_flag = 0;
    switch (keystroke_label)
    {
    case KEYSTROKE_ONE:
        cursor_row = (cursor_row > ROWS_MIN) ? cursor_row - 1 : ROWS_MAX; // ��������ƣ����cursorRow�ﵽ���ϲ㣬������ع����²�
        break;
    case KEYSTROKE_TWO:
        cursor_row = (cursor_row < ROWS_MAX) ? cursor_row + 1 : ROWS_MIN; // ��������ƣ����cursorRow�ﵽ���²㣬������ع����ϲ�
        break;
    case KEYSTROKE_THREE:
        menu_next_flag = 1;
        break;
    case KEYSTROKE_FOUR:
        menu_next_flag = -1;
        break;
    }

    lcd_showstr(0, cursor_row, ">");  // �� cursor_row ��Ӧλ�ô�ӡ��ͷ

    // ���֮ǰ��ͷλ�õ���ʾ���������
    if (previous_cursor_row != cursor_row)
    {
        lcd_showstr(0, previous_cursor_row, " ");  // �� previous_cursor_row ��Ӧλ�ô�ӡ�ո�
        previous_cursor_row = cursor_row;
    }
}

// �˵����¼���ת
void Menu_Next_Back()
{
    switch (menu_next_flag)
    {
    case 0:
        break;

    case -1: // ������һ��
        display_codename = display_codename / 10;
        cursor_row = ROWS_MIN;
        lcd_clear(WHITE);
        break;
    case 1: // ������һ��
        if (Have_Sub_Menu(display_codename * 10 + cursor_row))
        {
            display_codename = display_codename * 10 + cursor_row;
            lcd_clear(WHITE);
        }
        break;
    }

    menu_next_flag = 0; // �л���ҳ�棬��־λ��0
}

// ��鱾���Ƿ�����Ӳ˵�
int Have_Sub_Menu(int menu_id)
{
    uint8 i = 0;
    // sizeof(menu_have_sub) / sizeof(menu_have_sub [0]) �������鳤��
    for (i = 0; i <= sizeof(menu_have_sub) / sizeof(menu_have_sub[0]); i++)
    {
        if (menu_have_sub[i] == menu_id)
        {
            return 1;
        }
    }
    return 0;
}

// ������ɨ�践��ҳ������޸ı����߼�
void HandleKeystroke(int keystroke_label)
{
    switch (keystroke_label)
    {
    case KEYSTROKE_FOUR:
        display_codename /= 10; // ������һҳ
        lcd_clear(WHITE);
        break;
    case KEYSTROKE_THREE:
        keystroke_three_count++;
        switch (keystroke_three_count % 3)
        {
        case 0:
            change_unit_multiplier = 1;
            keystroke_three_count = 0;
            break;
        case 1:
            change_unit_multiplier = 10;
            break;
        case 2:
            change_unit_multiplier = 100;
            break;
        }
    break;
    }
}

// ���Ͳ����޸�
void Keystroke_int(int *parameter, int change_unit_MIN)
{
    int change_unit = change_unit_MIN * change_unit_multiplier;
    lcd_showint32(15 * 8, 0, change_unit, 3);

    Keystroke_Scan();
    HandleKeystroke(keystroke_label);

    switch (keystroke_label)
    {
    case KEYSTROKE_ONE:
        *parameter += change_unit;
        break;
    case KEYSTROKE_TWO:
        *parameter -= change_unit;
        break;
    }
}

// �����Ͳ����޸�
void Keystroke_float(float *parameter, float change_unit_MIN)
{
    float change_unit = change_unit_MIN * change_unit_multiplier;
    lcd_showfloat(14 * 8, 0, change_unit, 2, 3);
    
    Keystroke_Scan();
    HandleKeystroke(keystroke_label);

    switch (keystroke_label)
    {
    case KEYSTROKE_ONE:
        *parameter += change_unit;
        break;
    case KEYSTROKE_TWO:
        *parameter -= change_unit;
        break;
    }
}

// ������ֵ�޸ģ�-1��1
void Keystroke_Special_Value(int *parameter)
{
    Keystroke_Scan();
    HandleKeystroke(keystroke_label);

    switch (keystroke_label)
    {
    case KEYSTROKE_ONE:
        *parameter = -1;
        break;
    case KEYSTROKE_TWO:
        *parameter = 1;
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
    switch (display_codename)
    {
    case 0:
        Keystroke_Menu_HOME();
        break;

    case 1:
    case 11:
    case 12:
        Keystroke_Menu_ONE();
        break;

    case 2:
    case 21:
    case 22:
    case 23:
        Keystroke_Menu_TWO();
        break;

    default:
        break;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���˵�Ŀ¼
//  @param
//  @return     void
//  @note       ��ҳ��Ϊ���Ϊ0
//-------------------------------------------------------------------------------------------------------------------
void Keystroke_Menu_HOME(void) // 0
{
    while (menu_next_flag == 0)
    {
        lcd_showstr(CENTER_COLUMN - 2 * 8, 0, "MENU");
        lcd_showstr(1 * 8, 1, "STRAT");
        lcd_showstr(1 * 8, 2, "PID_SPEED");

        Keystroke_Scan();
        Cursor();
    }

    if (menu_next_flag == 1 && Have_Sub_Menu(cursor_row)) // ������һ���˵�
    {
        display_codename = cursor_row;
        cursor_row = ROWS_MIN;
        lcd_clear(WHITE);
    }

    else if (menu_next_flag == -1 && EEPROM_MODE == 1) // �����˵�ʱ���»��˼�������4��������eepromȷ��ˢд
    {
        eeprom_flash();

        // ˢд�����ʾ
        lcd_clear(WHITE);
        lcd_showstr(1 * 8, 1, "EEPROM_SAVED");
        lcd_showstr(1 * 8, 4, "@author Alaye_Dong"); // ���˾ͱ�ɾ.doge������
        delay_ms(400);

        BEEP_ON_ms(100);
        lcd_clear(WHITE);
    }

    menu_next_flag = 0; // �л���ҳ�棬��־λ��0
}

/*///////////////////////////////////////
   ��ҳ��
*/
///////////////////////////////////////
void Menu_ONE_Display(uint8 control_line)
{
    lcd_showstr(0 * 8, 0, "<<STRAT");

    lcd_showstr(1 * 8, 1, "Start_Flag");
    lcd_showstr(1 * 8, 2, "Out_Direction");

    lcd_showint32(14 * 8, 1, start_flag, 3);    // ��1�� Ӧ����ú���������ʱcontrol_line����һ�£�������ȷ��ʾ&��ʾ�ڵ����ı���
    lcd_showint32(14 * 8, 2, garage_out_direction, 3);

    lcd_showstr(0, control_line, "&"); //&��־��ʾ
}

void Keystroke_Menu_ONE(void) // 1 11 12
{
    switch (display_codename)
    {
    case 1:
        while (menu_next_flag == 0)
        {
            Menu_ONE_Display(-1);
            Keystroke_Scan();
            Cursor();   
        }
        Menu_Next_Back();
        break;

    case 11:
        Menu_ONE_Display(1);
        Keystroke_Special_Value(&start_flag);
        break;
    case 12:
        Menu_ONE_Display(2);
        Keystroke_Special_Value(&garage_out_direction);
        break;
    }

}

void Menu_TWO_Display(uint8 control_line)
{
    lcd_showstr(0 * 8, 0, "<<PID_SPEED");

    lcd_showstr(1 * 8, 1, "P");
    lcd_showstr(1 * 8, 2, "D");
    lcd_showstr(1 * 8, 3, "normal_speed");

    lcd_showfloat(14 * 8, 1, PID_P, 2, 3);
    lcd_showfloat(14 * 8, 2, PID_D, 2, 3);
    lcd_showint32(14 * 8, 3, normal_speed, 3);

    lcd_showstr(0, control_line, "&"); //&��־��ʾ
}

void Keystroke_Menu_TWO(void) // 2 21 22 23
{
    switch (display_codename)
    {
    case 2:
        while (menu_next_flag == 0)
        {
            Menu_TWO_Display(-1);
            Keystroke_Scan();
            Cursor();
        }
        Menu_Next_Back();
        break;

    case 21:
        Menu_TWO_Display(1);
        Keystroke_float(&PID_P, 0.001);
        break;
    case 22:
        Menu_TWO_Display(2);
        Keystroke_float(&PID_D, 0.001);
        break;
    case 23:
        Menu_TWO_Display(3);
        Keystroke_int(&normal_speed, 1);
        break;
    }
}
