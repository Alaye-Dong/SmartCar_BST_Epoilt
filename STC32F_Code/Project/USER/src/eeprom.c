#include "eeprom.h"

uint8 date_buff[100]; // eeprom��������
uint8 eeprom_init_time = 0;

void eeprom_init()
{
    iap_init(); // ��ʼ��EEPROM;

    iap_read_bytes(0x00, date_buff, 100); // ��EEPROM�ж�ȡ����

    eeprom_init_time = read_int(ADDRESS_EEPROM_INIT_TIME); // eepeomû�б���䣬����������ֵ

    if (eeprom_init_time != 1) // ����������eeprom_init_timeΪ����ֵ��if����
    {
        eeprom_init_time = 1;
        save_int(eeprom_init_time, ADDRESS_EEPROM_INIT_TIME); // ���eeprom_init_time��ֵ��eeprom

        eeprom_flash(); // ���Դ�������ʼ����ֵ��eeprom
    }
    else // �ǳ�����������ȡeeprom���ڸ�ֵ����
    {
        start_flag = read_int(ADDRESS_START_FLAG);
        garage_out_direction = read_int(ADDRESS_GARAGE_OUT_DIRECTION);
    }
}

// ˢд�������ݵ�eeprom
void eeprom_flash()
{
    save_int(start_flag, ADDRESS_START_FLAG);
    save_int(garage_out_direction, ADDRESS_GARAGE_OUT_DIRECTION);
}

/**
 * ����һ��������EEPROM�洢�С�
 *
 * �˺�����һ��int32���͵�����ֵ�ֽ�Ϊ�ֽڣ�������ָ����ֵλ��ʼ���浽date_buff�У�
 * ���ͨ��iap_write_bytes������date_buff������д��洢���ʡ�
 * ��Ҫ�������ض�λ�ø���EEPROM�洢�е����ݡ�
 *
 * @param input ��Ҫ�����������
 * @param value_bit ָ����ֵλ������ȷ��������date_buff�е���ʼ��ַλ�ã�ֵλ������EEPROM_ADDRESS_enmu�б�ö�١�
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