#ifndef __EEPROM_H_
#define __EEPROM_H_

#include "headfile.h"

// 枚举定义EEPROM地址
typedef enum EEPROM_ADDRESS
{
    ADDRESS_EEPROM_INIT_TIME = 0,
    ADDRESS_START_FLAG,
    ADDRESS_GARAGE_OUT_DIRECTION
    // 可以在这里添加更多的地址位
} EEPROM_ADDRESS_enmu;

extern uint8 date_buff[100]; // eeprom数据数组

void eeprom_init();
void eeprom_flash();

void save_int(int32 input, uint8 value_bit);
int32 read_int(uint8 value_bit);
void save_float(float input, uint8 value_bit);
float read_float(uint8 value_bit);

#endif