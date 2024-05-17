#include "headfile.h"

uint8 date_buff[100]; // eeprom数据数组
uint8 eeprom_init_time = 0;

void eeprom_init()
{
    iap_init(); // 初始化EEPROM;

    iap_read_bytes(0x00, date_buff, 100); // 从EEPROM中读取数据

    eeprom_init_time = read_int(0); // eepeom没有被填充，则会读到垃圾值

    if (eeprom_init_time != 1) // 初次启动，eeprom_init_time为垃圾值，if成立
    {
        eeprom_init_time = 1;
        save_int(eeprom_init_time, 0);  // 填充eeprom_init_time的值到eeprom

        eeprom_flash(); // 填充源码变量初始化的值到eeprom
    }
    else // 非初次启动，读取eeprom用于赋值变量
    {
        start_flag = read_int(1);
        garage_out_direction = read_int(2);
    }
}

// 刷写保存数据到eeprom
void eeprom_flash()
{
    save_int(start_flag, 1);
    save_int(garage_out_direction, 2);
}

void save_int(int32 input, uint8 value_bit)
{
    uint8 i;
    uint8 begin = value_bit * 4;
    uint8 *p = (uint8 *)&input;

    for (i = 0; i < 4; i++)
    {
        date_buff[begin++] = *(p + i);
    }
    extern_iap_write_bytes(0x00, date_buff, 100);
}

int32 read_int(uint8 value_bit)
{
    uint8 i;
    uint8 begin = value_bit * 4;
    int32 output;
    uint8 *p = (uint8 *)&output;

    for (i = 0; i < 4; i++)
    {
        *(p + i) = date_buff[begin++];
    }
    return output;
}

void save_float(float input, uint8 value_bit)
{
    uint8 i;
    uint8 begin = value_bit * 4;
    uint8 *p = (uint8 *)&input;

    for (i = 0; i < 4; i++)
    {
        date_buff[begin++] = *(p + i);
    }
    extern_iap_write_bytes(0x00, date_buff, 100);
}

float read_float(uint8 value_bit)
{
    uint8 i;
    uint8 begin = value_bit * 4;
    float output;
    uint8 *p = (uint8 *)&output;

    for (i = 0; i < 4; i++)
    {
        *(p + i) = date_buff[begin++];
    }
    return output;
}