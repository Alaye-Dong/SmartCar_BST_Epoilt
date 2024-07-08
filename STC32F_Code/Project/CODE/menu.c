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
 * @note
 *********************************************************************************************************************/

#include "menu.h"

// ���尴������
#define KEYSTROKE_ONE 1
#define KEYSTROKE_TWO 2
#define KEYSTROKE_THREE 3
#define KEYSTROKE_FOUR 4

#define ROWS_MAX 9                                        // �������Ļ�Ͽ��ƶ������������
#define ROWS_MIN 1                                        // �������Ļ�Ͽ��ƶ�������С����
#define CHAR_SCREEN_WIDTH 8                               // һ���ַ���ռ��Ļ�Ŀ�ȣ���λpx��
#define CENTER_COLUMN (TFT_X_MAX / CHAR_SCREEN_WIDTH / 2) // ������

#define EEPROM_MODE 1 // eeporm��д������Ϊ1

uint8 lcd_on_flag = 1;

int16 display_codename = 0;       // ��ʾҳ�����
int8 cursor_row = 0;              // ��������к�
int8 menu_next_flag = 0;          // �����ָ�˵������־λ
int16 change_unit_multiplier = 1; // �޸ĵ�λ����

// ��Ҫ���޸ĵĲ���ʾ��
int16 start_flag = 0, garage_out_direction = 0;

// ���в˵�ҳ��Ĵ�������������У���ֹ�ɼ�ͷ�����к����������벻���ڵĲ˵�
const int16 menu_have_sub[] = {
    0,
    1, 11, 12,
    2, 21, 22, 23, 24, 25, 26,
    3};

// �˵���ͷ��ʶ
void Cursor(void)
{
    static int8 previous_cursor_row = -1; // ��һ�ι�������к�

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

    lcd_showstr(0, cursor_row, ">"); // �� cursor_row ��Ӧλ�ô�ӡ��ͷ

    // ���֮ǰ��ͷλ�õ���ʾ���������
    if (previous_cursor_row != cursor_row)
    {
        lcd_showstr(0, previous_cursor_row, " "); // �� previous_cursor_row ��Ӧλ�ô�ӡ�ո�
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
            // if (Have_Sub_Menu(display_codename * 10 + cursor_row))
            // {
        display_codename = display_codename * 10 + cursor_row;
        lcd_clear(WHITE);
        // }
        break;
    }

    menu_next_flag = 0; // �л���ҳ�棬��־λ��0
}

// ��鱾���Ƿ�����Ӳ˵�
int16 Have_Sub_Menu(int16 menu_id)
{
    uint8 i = 0;
    // sizeof(menu_have_sub) / sizeof(menu_have_sub [0]) �������鳤��
    for (i = 0; i < sizeof(menu_have_sub) / sizeof(menu_have_sub[0]); i++)
    {
        if (menu_have_sub[i] == menu_id)
        {
            return 1;
        }
    }
    return 0;
}

// ������ҳ�水��ɨ�践��ҳ������޸ı����߼�
void HandleKeystroke(int16 keystroke_label)
{
    static uint8 keystroke_three_count = 0; // ��¼KEYSTROKE_THREE�Ĵ�������

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
    }
}

// ���Ͳ����޸�
void Keystroke_int(int16 *parameter, int16 change_unit_MIN)
{
    int16 change_unit = change_unit_MIN * change_unit_multiplier;
    lcd_showint32(10 * CHAR_SCREEN_WIDTH, 0, change_unit, 3);

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
    lcd_showfloat(10 * CHAR_SCREEN_WIDTH, 0, change_unit, 2, 3);

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
void Keystroke_Special_Value(int16 *parameter)
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
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
        Keystroke_Menu_ONE();
        break;

    case 2:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
        Keystroke_Menu_TWO();
        break;

    case 3:
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
    case 36:
    case 37:
    case 38:
    case 39:
        Keystroke_Menu_THREE();
        break;

    case 4:
    case 41:
    case 42:
    case 43:
    case 44:
    case 45:
    case 46:
    case 47:
    case 48:
    case 49:
        Keystroke_Menu_FOUR();
        break;

    default:
        display_codename /= 10; // ���û����һ�����򷵻ر���
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
    if (menu_next_flag == 0)
    {
        lcd_showstr((CENTER_COLUMN - 2) * CHAR_SCREEN_WIDTH, 0, "MENU");
        lcd_showstr(1 * CHAR_SCREEN_WIDTH, 1, "STRAT");
        lcd_showstr(1 * CHAR_SCREEN_WIDTH, 2, "PID_SET");
        lcd_showstr(1 * CHAR_SCREEN_WIDTH, 3, "PID_INFO");
        lcd_showstr(1 * CHAR_SCREEN_WIDTH, 4, "ELE_INFO");

        Keystroke_Scan();
        Cursor();
    }

    if (menu_next_flag == 1) // ������һ���˵� //&& Have_Sub_Menu(cursor_row)
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
        lcd_showstr(1 * CHAR_SCREEN_WIDTH, 1, "EEPROM_SAVED");
        lcd_showstr(1 * CHAR_SCREEN_WIDTH, 4, "@author Alaye_Dong"); // ���˾ͱ�ɾ.doge������
        delay_ms(400);

        BEEP_ON_ms(100);
        lcd_clear(WHITE);

        lcd_on_flag = 0;
    }

    menu_next_flag = 0; // �л���ҳ�棬��־λ��0
}

/*///////////////////////////////////////
   ��ҳ��
*/
///////////////////////////////////////
void Menu_ONE_Display(uint8 control_line)
{
    lcd_showstr(0 * CHAR_SCREEN_WIDTH, 0, "<<STRAT");

    lcd_showstr(1 * CHAR_SCREEN_WIDTH, 1, "Start_Flag");
    lcd_showstr(1 * CHAR_SCREEN_WIDTH, 2, "Out_Dir");

    lcd_showint32(11 * CHAR_SCREEN_WIDTH, 1, start_flag, 3); // ��1�� Ӧ����ú���������ʱcontrol_line����һ�£�������ȷ��ʾ&��ʾ�ڵ����ı���
    lcd_showint32(11 * CHAR_SCREEN_WIDTH, 2, garage_out_direction, 3);

    lcd_showstr(0, control_line, "&"); //&��־��ʾ
}

void Keystroke_Menu_ONE(void) // 1 11 12
{
    switch (display_codename)
    {
    case 1:
        if (menu_next_flag == 0)
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
    uint8 i = 0;
    lcd_showstr(0 * CHAR_SCREEN_WIDTH, i++, "<<PID_S");

    lcd_showstr(1 * CHAR_SCREEN_WIDTH, i, "dir.P");
    lcd_showfloat(9 * CHAR_SCREEN_WIDTH, i++, direction.KP, 2, 3);

    lcd_showstr(1 * CHAR_SCREEN_WIDTH, i, "dir.D");
    lcd_showfloat(9 * CHAR_SCREEN_WIDTH, i++, direction.KD, 2, 3);

    lcd_showstr(1 * CHAR_SCREEN_WIDTH, i, "mot_L.P");
    lcd_showfloat(9 * CHAR_SCREEN_WIDTH, i++, motor_left.KP, 2, 3);

    lcd_showstr(1 * CHAR_SCREEN_WIDTH, i, "mot_L.I");
    lcd_showfloat(9 * CHAR_SCREEN_WIDTH, i++, motor_left.KI, 2, 3);

    lcd_showstr(1 * CHAR_SCREEN_WIDTH, i, "mot_R.P");
    lcd_showfloat(9 * CHAR_SCREEN_WIDTH, i++, motor_right.KP, 2, 3);

    lcd_showstr(1 * CHAR_SCREEN_WIDTH, i, "mot_R.I");
    lcd_showfloat(9 * CHAR_SCREEN_WIDTH, i++, motor_right.KI, 2, 3);

    lcd_showstr(0, control_line, "&"); //&��־��ʾ
}

void Keystroke_Menu_TWO(void) // 2 21 22 23
{
    switch (display_codename)
    {
    case 2:
        if (menu_next_flag == 0)
        {
            Menu_TWO_Display(-1);
            Keystroke_Scan();
            Cursor();
        }
        Menu_Next_Back();
        break;

    case 21:
        Menu_TWO_Display(1);
        Keystroke_float(&direction.KP, 0.001);
        break;
    case 22:
        Menu_TWO_Display(2);
        Keystroke_float(&direction.KD, 0.001);
        break;
    case 23:
        Menu_TWO_Display(3);
        Keystroke_float(&motor_left.KP, 0.001);
        break;
    case 24:
        Menu_TWO_Display(4);
        Keystroke_float(&motor_left.KI, 0.001);
        break;
    case 25:
        Menu_TWO_Display(5);
        Keystroke_float(&motor_right.KP, 0.001);
        break;
    case 26:
        Menu_TWO_Display(6);
        Keystroke_float(&motor_right.KI, 0.001);
        break;
    }
}

void Menu_THREE_Display(uint8 control_line)
{
    uint8 i = 0;
    lcd_showstr(0 * CHAR_SCREEN_WIDTH, i, "<<INFO");
    lcd_showint32(6 * CHAR_SCREEN_WIDTH, i, gyro_z_filtered, 4);
    lcd_showfloat(10 * CHAR_SCREEN_WIDTH, i, yaw_angle, 3, 1);
    i++;

    lcd_showint32(0 * CHAR_SCREEN_WIDTH, i, encoder_left.encoder_now, 4);
    lcd_showstr(6 * CHAR_SCREEN_WIDTH, i, "EC");
    lcd_showint32(11 * CHAR_SCREEN_WIDTH, i++, encoder_right.encoder_now, 4);

    lcd_showint32(0 * CHAR_SCREEN_WIDTH, i, speed.target, 4);
    lcd_showstr(6 * CHAR_SCREEN_WIDTH, i, "V/Pos");
    lcd_showint32(11 * CHAR_SCREEN_WIDTH, i++, position, 4);

    lcd_showfloat(0 * CHAR_SCREEN_WIDTH, i, direction.KP, 2, 3);
    lcd_showstr(6 * CHAR_SCREEN_WIDTH, i, "dPD");
    lcd_showfloat(10 * CHAR_SCREEN_WIDTH, i++, direction.KD, 2, 3);

    lcd_showfloat(0 * CHAR_SCREEN_WIDTH, i, direction.KP_2, 2, 3);
    lcd_showstr(6 * CHAR_SCREEN_WIDTH, i, "dPD2");
    lcd_showfloat(10 * CHAR_SCREEN_WIDTH, i++, direction.KD_2, 2, 3);

    // lcd_showfloat(1 * CHAR_SCREEN_WIDTH, i, direction.KF, 2, 3);
    // lcd_showstr(8 * CHAR_SCREEN_WIDTH, i++, "dF");

    lcd_showint32(0 * CHAR_SCREEN_WIDTH, i, motor_left_pwm, 4);
    lcd_showstr(6 * CHAR_SCREEN_WIDTH, i, "PWM");
    lcd_showint32(11 * CHAR_SCREEN_WIDTH, i++, motor_right_pwm, 4);

    lcd_showfloat(1 * CHAR_SCREEN_WIDTH, i, motor_left.KP, 2, 3);
    lcd_showstr(8 * CHAR_SCREEN_WIDTH, i, "MP");
    lcd_showfloat(10 * CHAR_SCREEN_WIDTH, i++, motor_right.KP, 2, 3);

    lcd_showfloat(1 * CHAR_SCREEN_WIDTH, i, motor_left.KI, 2, 3);
    lcd_showstr(8 * CHAR_SCREEN_WIDTH, i, "MI");
    lcd_showfloat(10 * CHAR_SCREEN_WIDTH, i++, motor_right.KI, 2, 3);

    // lcd_showint32(1 * CHAR_SCREEN_WIDTH, i, speed.target_left, 4);
    // lcd_showstr(6 * CHAR_SCREEN_WIDTH, i, "MTar");
    // lcd_showint32(11 * CHAR_SCREEN_WIDTH, i++, speed.target_right, 4);

    // lcd_showfloat(1 * CHAR_SCREEN_WIDTH, i, motor_left.KF, 2, 3);
    // lcd_showstr(8 * CHAR_SCREEN_WIDTH, i, "MF");
    // lcd_showfloat(10 * CHAR_SCREEN_WIDTH, i++, motor_right.KF, 2, 3);

    lcd_showint32(0 * CHAR_SCREEN_WIDTH, i, position_vector_modulus, 3);
    lcd_showint32(6 * CHAR_SCREEN_WIDTH, i, position_normal, 3);
    // lcd_showstr(6 * CHAR_SCREEN_WIDTH, i, "VeDi");
    lcd_showint32(11 * CHAR_SCREEN_WIDTH, i++, position_difference_weighted, 3);

    lcd_showstr(0, control_line, "&"); //&��־��ʾ
}

void Keystroke_Menu_THREE(void) // 3
{
    switch (display_codename)
    {
    case 3:
        if (menu_next_flag == 0)
        {
            Menu_THREE_Display(-1);
            Keystroke_Scan();
            Cursor();
        }
        Menu_Next_Back();
        break;
    }
}

// ��û�м���˵�ϵͳ�������ⲿ����
void Menu_FOUR_Display(uint8 control_line)
{
    uint8 i = 0;
    lcd_showstr(0 * CHAR_SCREEN_WIDTH, i, "<<ELE");
    lcd_showint32(6 * CHAR_SCREEN_WIDTH, i, gyro_z_filtered, 4);
    lcd_showfloat(10 * CHAR_SCREEN_WIDTH, i++, yaw_angle, 3, 1);

    lcd_showint32(1 * CHAR_SCREEN_WIDTH, i, ADC_values[LEFT_V][0], 4);
    lcd_showstr(8 * CHAR_SCREEN_WIDTH, i, "|");
    lcd_showint32(11 * CHAR_SCREEN_WIDTH, i++, ADC_values[RIGHT_V][0], 4);
    lcd_showint32(1 * CHAR_SCREEN_WIDTH, i, ADC_values[LEFT_H][0], 4);
    lcd_showstr(8 * CHAR_SCREEN_WIDTH, i, "-");
    lcd_showint32(11 * CHAR_SCREEN_WIDTH, i++, ADC_values[RIGHT_H][0], 4);

    lcd_showint32(1 * CHAR_SCREEN_WIDTH, i, inductor[LEFT_V], 4);
    lcd_showstr(8 * CHAR_SCREEN_WIDTH, i, "|");
    lcd_showint32(11 * CHAR_SCREEN_WIDTH, i++, inductor[RIGHT_V], 4);
    lcd_showint32(1 * CHAR_SCREEN_WIDTH, i, inductor[LEFT_H], 4);
    lcd_showstr(8 * CHAR_SCREEN_WIDTH, i, "-");
    lcd_showint32(11 * CHAR_SCREEN_WIDTH, i++, inductor[RIGHT_H], 4);

    lcd_showint32(0 * CHAR_SCREEN_WIDTH, i, speed.target, 4);
    lcd_showstr(6 * CHAR_SCREEN_WIDTH, i, "V/Dir");
    lcd_showint32(11 * CHAR_SCREEN_WIDTH, i++, position, 4);

    lcd_showint32(0 * CHAR_SCREEN_WIDTH, i, motor_left_pwm, 4);
    lcd_showstr(6 * CHAR_SCREEN_WIDTH, i, "PWM");
    lcd_showint32(11 * CHAR_SCREEN_WIDTH, i++, motor_right_pwm, 4);

    lcd_showint32(0 * CHAR_SCREEN_WIDTH, i, dl1b_distance_mm, 5);
    lcd_showstr(6 * CHAR_SCREEN_WIDTH, i, "ToDs");
    lcd_showint32(11 * CHAR_SCREEN_WIDTH, i++, car_distance_cm, 4);

    // lcd_showint32(0 * CHAR_SCREEN_WIDTH, i, round_left_proximity_coeff, 4);
    // lcd_showstr(6 * CHAR_SCREEN_WIDTH, i, "Rc");
    // lcd_showint32(8 * CHAR_SCREEN_WIDTH, i, round_flag, 1);
    // lcd_showint32(11 * CHAR_SCREEN_WIDTH, i++, round_right_proximity_coeff, 4);

    lcd_showint32(0 * CHAR_SCREEN_WIDTH, i, position_vector_modulus, 3);
    lcd_showint32(6 * CHAR_SCREEN_WIDTH, i, position_normal, 3);
    // lcd_showstr(6 * CHAR_SCREEN_WIDTH, i, "VeDi");
    lcd_showint32(11 * CHAR_SCREEN_WIDTH, i++, position_difference_weighted, 3);

    lcd_showint32(0 * CHAR_SCREEN_WIDTH, i, round_left_turn_in_proximity_coeff, 4);
    lcd_showstr(6 * CHAR_SCREEN_WIDTH, i, "TI");
    lcd_showint32(11 * CHAR_SCREEN_WIDTH, i++, round_right_turn_in_proximity_coeff, 4);

    lcd_showstr(0, control_line, "&"); //&��־��ʾ
}

void Keystroke_Menu_FOUR(void) // 4
{
    switch (display_codename)
    {
    case 4:
        if (menu_next_flag == 0)
        {
            Menu_FOUR_Display(-1);
            Keystroke_Scan();
            Cursor();
        }
        Menu_Next_Back();
        break;
    }
}