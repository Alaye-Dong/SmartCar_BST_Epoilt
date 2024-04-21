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

// 定义按键代号
#define KEYSTROKE_ONE 1
#define KEYSTROKE_TWO 2
#define KEYSTROKE_THREE 3
#define KEYSTROKE_FOUR 4

#define ROWS_MAX 7           // 光标在屏幕上可移动至的最大行数
#define ROWS_MIN 1           // 光标在屏幕上可移动至的最小行数
#define CENTER_COLUMN 10 * 8 // 中央列
#define EEPROM_MODE 1        // eeporm读写开启则为1

int display_codename = 0; // 显示页面代号
uint8 cursor_row = 0;     // 光标所在行号
uint8 previous_cursor_row = 0;  // 上一次光标所在列号
uint8 menu_next_flag = 0; // 光标所指菜单进入标志位
float change_unit = 0;    // 单次修改的单位值

// 需要被修改的参数示例
int start_flag = 0, garage_out_direction = 0;
float PID_P = 0.123, PID_D = 0.123;
int normal_speed = 0;

// 将有菜单页面的代号填入该数组中，防止由箭头所在行号所决定进入不存在的菜单
int menu_have_sub[] = {
    0,
    1, 11, 12,
    2, 21, 22, 23};

// 菜单箭头标识
void Cursor(void)
{
    menu_next_flag = 0;

    

    switch (keystroke_label)
    {
    case KEYSTROKE_ONE:
        cursor_row = (cursor_row > ROWS_MIN) ? cursor_row - 1 : ROWS_MAX; // 光标行上移，如果cursorRow达到最上层，再上则回归最下层
        break;
    case KEYSTROKE_TWO:
        cursor_row = (cursor_row < ROWS_MAX) ? cursor_row + 1 : ROWS_MIN; // 光标行下移，如果cursorRow达到最下层，再下则回归最上层
        break;
    case KEYSTROKE_THREE:
        menu_next_flag = 1;
        break;
    case KEYSTROKE_FOUR:
        menu_next_flag = -1;
        break;
    }

    lcd_showstr(0, cursor_row, ">");  // 在 cursor_row 对应位置打印箭头

    // 清除之前箭头位置的显示，避免残留
    if (previous_cursor_row != cursor_row)
    {
        lcd_showstr(0, previous_cursor_row, " ");  // 在 previous_cursor_row 对应位置打印空格
        previous_cursor_row = cursor_row;
    }
}

// 菜单上下级跳转
void Menu_Next_Back()
{
    switch (menu_next_flag)
    {
    case 0:
        break;

    case -1: // 返回上一级
        display_codename = display_codename / 10;
        cursor_row = 0;
        lcd_clear(WHITE);
        break;
    case 1: // 进入下一级
        if (Have_Sub_Menu(display_codename * 10 + cursor_row))
        {
            display_codename = display_codename * 10 + cursor_row;
            cursor_row = 0;
            lcd_clear(WHITE);
        }
        break;
    }

    menu_next_flag = 0; // 切换完页面，标志位归0
}

// 检查本行是否存在子菜单
int Have_Sub_Menu(int menu_id)
{
    uint8 i = 0;
    // sizeof(menu_have_sub) / sizeof(menu_have_sub [0]) 计算数组长度
    for (i = 0; i <= sizeof(menu_have_sub) / sizeof(menu_have_sub[0]); i++)
    {
        if (menu_have_sub[i] == menu_id)
        {
            return 1;
        }
    }
    return 0;
}

// 处理按键扫描返回页逻辑
void HandleKeystroke(int keystroke_label)
{
    switch (keystroke_label)
    {
    case KEYSTROKE_THREE:
        display_codename /= 10; // 返回上一页
        lcd_clear(WHITE);
        break;
    case KEYSTROKE_FOUR:
        display_codename = 0; // 返回第0页
        lcd_clear(WHITE);
        break;
    }
}

// 整型参数修改
void Keystroke_int(int *parameter, int change_unit_MIN)
{
    int change_unit = change_unit_MIN;
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

// 浮点型参数修改
void Keystroke_float(float *parameter, float change_unit_MIN)
{
    float change_unit = change_unit_MIN;
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

// 整型特值修改，-1或1
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
//  @brief      菜单目录
//  @param
//  @return     void
//  @note       启用while来显示目标页面  在每个页面按键按键后都会改变到对应页面，此函数用于更新屏幕显示
//             增删页的同时请记得同步修改menu_have_sub[]数组的值
//-------------------------------------------------------------------------------------------------------------------
void Keystroke_Menu(void)
{
    switch (display_codename)
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

    default:
        break;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      主菜单目录
//  @param
//  @return     void
//  @note       此页面为编号为0
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

    if (menu_next_flag == 1 && Have_Sub_Menu(cursor_row)) // 进入下一级菜单
    {
        display_codename = cursor_row;
        cursor_row = 0;
        lcd_clear(WHITE);
    }

    else if (menu_next_flag == -1 && EEPROM_MODE == 1) // 在主菜单时按下回退键（按键4）来进行eeprom确认刷写
    {
        eeprom_flash();

        // 刷写完成提示
        lcd_clear(WHITE);
        lcd_showstr(1 * 8, 1, "EEPROM_SAVED");
        lcd_showstr(1 * 8, 4, "@author Alaye_Dong"); // 用了就别删.doge！！！

        delay_ms(400);
        BEEP = 1;
        delay_ms(100);
        BEEP = 0;
        lcd_clear(WHITE);
    }

    menu_next_flag = 0; // 切换完页面，标志位归0
}

/*///////////////////////////////////////
   子页面
*/
///////////////////////////////////////
void Menu_ONE_Display(uint8 control_line)
{
    lcd_showstr(CENTER_COLUMN - 2 * 8, 0, "STRAT");
    lcd_showstr(1 * 8, 1, "Start_Flag");
    lcd_showstr(1 * 8, 2, "Out_Direction");

    lcd_showint32(14 * 8, 1, start_flag, 3);    // “1” 应该与该函数被调用时control_line参数一致，才能正确显示&表示在调整的变量
    lcd_showint32(14 * 8, 2, garage_out_direction, 3);

    lcd_showstr(0, control_line, "&"); //&标志提示
}

void Keystroke_Menu_ONE(void) // 1
{
    while (menu_next_flag == 0)
    {
        Menu_ONE_Display(-1);
        Keystroke_Scan();
        Cursor();   //Cursor()中调用了按键扫描可以脱离循环
    }
    Menu_Next_Back();
}

void Keystroke_Menu_ONE_One(void) // 11
{
    Menu_ONE_Display(1);
    Keystroke_Special_Value(&start_flag);
}

void Keystroke_Menu_ONE_Two(void) // 12
{
    Menu_ONE_Display(2);
    Keystroke_Special_Value(&garage_out_direction);
}

/*////////////////////////////////////
    PD
*/
///////////////////////////////////
void Menu_TWO_Display(uint8 control_line)
{
    lcd_showstr(CENTER_COLUMN - 4 * 8, 0, "PID_SPEED");
    lcd_showstr(1 * 8, 1, "P");
    lcd_showstr(1 * 8, 2, "D");
    lcd_showstr(1 * 8, 3, "normal_speed");

    lcd_showfloat(14 * 8, 1, PID_P, 2, 3);
    lcd_showfloat(14 * 8, 2, PID_D, 2, 3);
    lcd_showint32(14 * 8, 3, normal_speed, 3);

    lcd_showstr(0, control_line, "&"); //&标志提示
}

void Keystroke_Menu_TWO(void) // 2
{
    while (menu_next_flag == 0)
    {
        Menu_TWO_Display(-1);
        Keystroke_Scan();
        Cursor();
    }
    Menu_Next_Back();
}

void Keystroke_Menu_TWO_One(void) // 21
{
    Menu_TWO_Display(1);
    Keystroke_float(&PID_P, 0.001);
}
void Keystroke_Menu_TWO_Two(void) // 22
{
    Menu_TWO_Display(2);
    Keystroke_float(&PID_D, 0.001);
}
void Keystroke_Menu_TWO_Three(void) // 23
{
    Menu_TWO_Display(3);
    Keystroke_int(&normal_speed, 1);
}
