#ifndef _MENU_H_
#define _MENU_H_

#include "headfile.h"

//定义按键代号
#define KeystrokeONE 1
#define KeystrokeTWO 2
#define KeystrokeTHREE 3
#define KeystrokeFOUR 4

#define RowsMax 7 //光标在屏幕上可移动至的最大行数
#define RowsMin 1 //光标在屏幕上可移动至的最小行数
#define CenterColumn 10*8   //中央列
#define EEPROM_MODE 1 //eeporm读写开启则为1

extern int displayCodename;       //显示页面代号
extern uint8 cursorRow; //光标所在行号

extern int Start_Flag, garage_out_direction;
extern float PID_P, PID_D;
extern int normal_speed;

void Cursor(void);
void Menu_Next_Back(void);
int have_sub_menu(int menu_id); // 查看是否存在子菜单

void Keystroke_Special_Value(int* parameter);
void Keystroke_int(int* parameter, int change_unit_MIN);
void Keystroke_float(float* parameter, float change_unit_MIN);

void Keystroke_Menu(void);
void Keystroke_Menu_HOME(void);

void Menu_ONE_Display(uint8 control_line);
void Keystroke_Menu_ONE(void);
void Keystroke_Menu_ONE_One(void);
void Keystroke_Menu_ONE_Two(void); 

void Menu_TWO_Display(uint8 control_line);
void Keystroke_Menu_TWO(void);
void Keystroke_Menu_TWO_One(void);
void Keystroke_Menu_TWO_Two(void);
void Keystroke_Menu_TWO_Three(void);

#endif