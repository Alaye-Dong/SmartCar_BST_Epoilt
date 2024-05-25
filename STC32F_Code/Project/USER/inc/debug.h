#ifndef __DEBUG_H
#define __DEBUG_H

#include "headfile.h"

extern uint8 print_send_flag;

void Wireless_Debug_Init(void);
void Wireless_Seekfree_Assistant_Debug(void);
void Debug_Parameter_Assignment(uint8 i);
void Debug_Parameter_Print(void);
uint32 seekfree_assistant_transfer_callback(const uint8 *buff, uint32 length);
void Debug_Parameter_Oscilloscope_Send(void);

#endif