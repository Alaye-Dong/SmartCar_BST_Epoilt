#include "eeprom.h"

uint8 date_buff[100]; // eeprom数据数组
uint8 eeprom_init_time = 0;

void eeprom_init()
{
    iap_init(); // 初始化EEPROM;

    iap_read_bytes(0x00, date_buff, 100); // 从EEPROM中读取数据

    eeprom_init_time = read_int(ADDRESS_EEPROM_INIT_TIME); // eepeom没有被填充，则会读到垃圾值

    if (eeprom_init_time != 1) // 初次启动，eeprom_init_time为垃圾值，if成立
    {
        eeprom_init_time = 1;
        save_int(eeprom_init_time, ADDRESS_EEPROM_INIT_TIME); // 填充eeprom_init_time的值到eeprom

        eeprom_flash(); // 填充源码变量初始化的值到eeprom
    }
    else // 非初次启动，读取eeprom用于赋值变量
    {
        start_flag = read_int(ADDRESS_START_FLAG);
        garage_out_direction = read_int(ADDRESS_GARAGE_OUT_DIRECTION);
    }
}

// 刷写保存数据到eeprom
void eeprom_flash()
{
    save_int(start_flag, ADDRESS_START_FLAG);
    save_int(garage_out_direction, ADDRESS_GARAGE_OUT_DIRECTION);
}

/**
 * 保存一个整数到EEPROM存储中。
 *
 * 此函数将一个int32类型的输入值分解为字节，并根据指定的值位开始保存到date_buff中，
 * 最后通过iap_write_bytes函数将date_buff的内容写入存储介质。
 * 主要用于在特定位置更新EEPROM存储中的数据。
 *
 * @param input 需要保存的整数。
 * @param value_bit 指定的值位，用于确定数据在date_buff中的起始地址位置，值位分配在EEPROM_ADDRESS_enmu中被枚举。
 */
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