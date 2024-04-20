#ifndef _KEY_H_
#define _KEY_H_

#include "headfile.h"

//定义按键引脚
#define KEY1_PIN    P50
#define KEY2_PIN    P51
#define KEY3_PIN    P16
#define KEY4_PIN    P17
//定义拨码开关引??
#define SW1_PIN     P76
#define SW2_PIN     P75

//拨码开关状态变量
extern uint8 sw1_status;
extern uint8 sw2_status;

extern uint8 keystrokeLabel;

void Keystroke_Scan(void);

#endif
