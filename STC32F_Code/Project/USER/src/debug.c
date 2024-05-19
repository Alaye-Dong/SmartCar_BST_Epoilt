#include "debug.h"

#define TEMP_BUFFER_SIZE 64
static uint8 temp_uart_buffer[TEMP_BUFFER_SIZE]; // ���ݴ������

char send_str[32] = {0};

void Wireless_Debug_Init(void)
{
    // ��ʼ������ת����
    wireless_uart_init();

    // ���ú���ָ��
    seekfree_assistant_receive = wireless_uart_read_buff;

    seekfree_assistant_init();
}

void Wireless_Debug_Main(void)
{
    uint8 i = 0;

    // �������ֽ������յ�������
    seekfree_assistant_data_analysis();

    // ����
    for (i = 0; i < SEEKFREE_ASSISTANT_SET_PARAMETR_COUNT; i++)
    {
        // ���±�־λ
        if (seekfree_assistant_parameter_update_flag[i])
        {
            seekfree_assistant_parameter_update_flag[i] = 0;

            // ����ͨ����Ϣ
            sprintf(send_str, "\r\nreceive data channel : %d  ", i);
            wireless_uart_send_buff((uint8 *)send_str, strlen(send_str));

            // ���֮ǰ�洢���ַ���
            memset(send_str, 0, strlen(send_str));

            // �����յ������ݸ�ֵ����Ӧ����
            Debug_Parameter_Assignment(i);

            // ���ͽ��յ�������
            func_float_to_str(send_str, seekfree_assistant_parameter[i], 2);
            wireless_uart_send_buff("data : ", strlen("data : "));
            wireless_uart_send_buff((uint8 *)send_str, strlen(send_str));

            // ���֮ǰ�洢���ַ���
            memset(send_str, 0, strlen(send_str));
        }
    }
}

/**
 * @description: 
 * @param {uint8} i ��Ӧ���������0~7��8��ͨ��
 * @return {*}
 */
void Debug_Parameter_Assignment(uint8 i)
{
    switch (i)
    {
    case 0:
        motor_left.KP = seekfree_assistant_parameter[i];
        break;
    case 1:
        motor_left.KI = seekfree_assistant_parameter[i];
        break;
    case 2:
        motor_right.KP = seekfree_assistant_parameter[i];
        break;
    case 3:
        motor_right.KI = seekfree_assistant_parameter[i];
        break;
    case 4:
        direction.KP = seekfree_assistant_parameter[i];
        break;
    case 5:
        direction.KD = seekfree_assistant_parameter[i];
        break;
        
    case 6:
        break;
    case 7:
        break;
    default:
        break;
    }
}
