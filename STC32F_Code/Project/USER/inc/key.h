#ifndef _KEY_H_
#define _KEY_H_

#include "headfile.h"

//拨码开关状态变量
extern uint8 keystrokeLabel;
extern uint8 sw1_status;
extern uint8 sw2_status;



void Keystroke_Scan(void);

#endif
