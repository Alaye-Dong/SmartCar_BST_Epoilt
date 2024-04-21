#include "headfile.h"

uint8 date_buff[100]; // eeprom数据数组
uint8 eeprom_init_time = 0;
void eeprom_init()
{
    iap_init(); // 初始化EEPROM;

    iap_read_bytes(0x00, date_buff, 100); // 从EEPROM中读取数据

    // 刷写程序后初次启动,使用源码初始化的值填充eeprom
    eeprom_init_time = read_int(0);
    if (eeprom_init_time != 1)
    {
        eeprom_init_time++;
        save_int(eeprom_init_time, 0);

        eeprom_flash();
    }
    else
    {
        // eeprom读取,用eeprom赋值变量
        start_flag = read_int(1);
        garage_out_direction = read_int(2);
        PID_P = read_float(3);
        PID_D = read_float(4);
        normal_speed = read_int(5);
    }
}

// 刷写保存数据到eeprom
void eeprom_flash()
{
    save_int(start_flag, 1);
    save_int(garage_out_direction, 2);
    save_float(PID_P, 3);
    save_float(PID_D, 4);
    save_int(normal_speed, 5);
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