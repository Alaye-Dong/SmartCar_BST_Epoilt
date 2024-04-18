#include "headfile.h"

uint8 date_buff[100]; //eeprom��������
uint8 eeprom_init_time = 0;
void eeprom_init()
{
    iap_init();									//��ʼ��EEPROM;

    iap_read_bytes(0x00, date_buff, 100);		//��EEPROM�ж�ȡ����

/****����eepromֱ�Ӹ�ֵ****/

	
/****��eeprom��ֵ****/
    
    //ˢд������������,���eeprom
    eeprom_init_time = read_int(0);	
    if(eeprom_init_time != 1)
    {
        save_int(1,0);
        
        save_int(Start_Flag,1);
        save_int(garage_out_direction,2);
        save_float(PID_P,3);
        save_float(PID_D,4);
        save_int(normal_speed,5);
    }
    
    //eeprom��ȡ
    Start_Flag = read_int(1);
    garage_out_direction = read_int(2);
	PID_P = read_float(3);
    PID_D = read_float(4);
    normal_speed = read_int(5);
}

void eeprom_flash()
{
    //��Ҫ����Ĳ���
    save_int(Start_Flag,1);
    save_int(garage_out_direction,2);
    save_float(PID_P,3);
    save_float(PID_D,4);
    save_int(normal_speed,5);
}

void save_int(int32 input, uint8 value_bit)
{
    uint8 i;
    uint8 begin = value_bit * 4;
    uint8* p = (uint8*)&input;
    for(i = 0; i < 4; i++)
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
    uint8* p = (uint8*)&output;;
    for(i = 0; i < 4; i++)
    {
        *(p + i) = date_buff[begin++];
    }
    return output;
}

void save_float(float input, uint8 value_bit)
{
    uint8 i;
    uint8 begin = value_bit * 4;
    uint8* p = (uint8*)&input;
    for(i = 0; i < 4; i++)
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
    uint8* p = (uint8*)&output;;
    for(i = 0; i < 4; i++)
    {
        *(p + i) = date_buff[begin++];
    }
    return output;
}