#ifndef _MENU_H_
#define _MENU_H_

#include "headfile.h"

extern int16 display_codename; // 显示页面代号

extern int16 start_flag, garage_out_direction;

void Cursor(void);
void Menu_Next_Back(void);
int16 Have_Sub_Menu(int16 menu_id); // 查看是否存在子菜单

void HandleKeystroke(int16 keystroke_label);
void Keystroke_Special_Value(int16 *parameter);
void Keystroke_int(int16 *parameter, int16 change_unit_MIN);
void Keystroke_float(float *parameter, float change_unit_MIN);

void Keystroke_Menu(void);
void Keystroke_Menu_HOME(void);

void Menu_ONE_Display(uint8 control_line);
void Keystroke_Menu_ONE(void);

void Menu_TWO_Display(uint8 control_line);
void Keystroke_Menu_TWO(void);

void Menu_THREE_Display(uint8 control_line);
void Keystroke_Menu_THREE(void);

#endif