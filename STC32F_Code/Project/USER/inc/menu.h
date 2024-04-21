#ifndef _MENU_H_
#define _MENU_H_

#include "headfile.h"

extern int display_codename; // ��ʾҳ�����

extern int start_flag, garage_out_direction;
extern float PID_P, PID_D;
extern int normal_speed;

void Cursor(void);
void Menu_Next_Back(void);
int Have_Sub_Menu(int menu_id); // �鿴�Ƿ�����Ӳ˵�

void HandleKeystroke(int keystroke_label);
void Keystroke_Special_Value(int *parameter);
void Keystroke_int(int *parameter, int change_unit_MIN);
void Keystroke_float(float *parameter, float change_unit_MIN);

void Keystroke_Menu(void);
void Keystroke_Menu_HOME(void);

void Menu_ONE_Display(uint8 control_line);
void Keystroke_Menu_ONE(void);

void Menu_TWO_Display(uint8 control_line);
void Keystroke_Menu_TWO(void);

#endif