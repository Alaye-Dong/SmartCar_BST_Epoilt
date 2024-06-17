#include "debug.h"

char send_str[32] = {0};
uint8 print_send_flag = 0;

void Wireless_Debug_Init(void)
{
    // ��ʼ������ת����
    wireless_uart_init();

    // ���ú���ָ��
    seekfree_assistant_receive = wireless_uart_read_buff;

    // ���ú���ָ�� Debug_Parameter_Oscilloscope_Send()
    seekfree_assistant_transfer = seekfree_assistant_transfer_callback;
    // ��Ҫ��������ʾ����ͨ����
    seekfree_assistant_oscilloscope_data.channel_num = 8;

    seekfree_assistant_init();
}

void Wireless_Seekfree_Assistant_Debug(void)
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

            // // ����ͨ����Ϣ
            // sprintf(send_str, "\r\nreceive data channel : %d  ", i);
            // wireless_uart_send_buff((uint8 *)send_str, strlen(send_str));

            // // ���֮ǰ�洢���ַ���
            // memset(send_str, 0, strlen(send_str));

            // �����յ������ݸ�ֵ����Ӧ����
            Debug_Parameter_Assignment(i);

            // // ���ͽ��յ�������
            // func_float_to_str(send_str, seekfree_assistant_parameter[i], 2);
            // wireless_uart_send_buff("data : ", strlen("data : "));
            // wireless_uart_send_buff((uint8 *)send_str, strlen(send_str));

            // // ���֮ǰ�洢���ַ���
            // memset(send_str, 0, strlen(send_str));
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
        // case 0:
        //     speed.normal = seekfree_assistant_parameter[i];
        //     break;
        // case 1:
        //     direction.KF = seekfree_assistant_parameter[i];
        //     break;
        // case 2:
        //     motor_left.KF = seekfree_assistant_parameter[i];
        //     break;
        // case 3:
        //     motor_right.KF = seekfree_assistant_parameter[i];
        //     break;
        // case 4:
        //     direction.KP = seekfree_assistant_parameter[i];
        //     break;
        // case 5:
        //     direction.KD = seekfree_assistant_parameter[i];
        //     break;
        // case 6:
        //     direction.KP_2 = seekfree_assistant_parameter[i];
        //     break;
        // case 7:
        //     direction.KD_2 = seekfree_assistant_parameter[i];
        //     break;

    case 0:
        speed.normal = seekfree_assistant_parameter[i];
        break;
    case 1:
        motor_left.KP = seekfree_assistant_parameter[i];
        break;
    case 2:
        motor_left.KI = seekfree_assistant_parameter[i];
        break;
    case 3:
        motor_right.KP = seekfree_assistant_parameter[i];
        break;
    case 4:
        direction.KP = seekfree_assistant_parameter[i];
        break;
    case 5:
        direction.KF = seekfree_assistant_parameter[i];
        break;
    case 6:
        motor_right.KI = seekfree_assistant_parameter[i];
        break;
    case 7:
        direction.KD_2 = seekfree_assistant_parameter[i];
        break;

    default:
        break;
    }
}

void Debug_Parameter_Print()
{
    if (print_send_flag)
    {
        print_send_flag = 0;
        printf("%d,", encoder_left.encoder_now);
        printf("%d\n", encoder_right.encoder_now);
        // sprintf(print_send_str,"%d\n", encoder_left.encoder_now);
        // wireless_uart_send_buff((uint8 *)print_send_str,strlen(print_send_str));
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ������ַ��ͺ���
// ����˵��     *buff           ��Ҫ���͵����ݵ�ַ
// ����˵��     length          ��Ҫ���͵ĳ���
// ���ز���     uint32          ʣ��δ�������ݳ���
// ʹ��ʾ��
//-------------------------------------------------------------------------------------------------------------------
uint32 seekfree_assistant_transfer_callback(const uint8 *buff, uint32 length)
{
    uart_putbuff(WIRELESS_UART, buff, length);
    return 0;
}

/**
 * @note: ����ͨ����������seekfree_assistant_oscilloscope_data.channel_num����������Wireless_Debug_Init()�б���ֵ
 * @return {*}
 */
void Debug_Parameter_Oscilloscope_Send(void)
{
    // ! ������Ҫ���������ݣ���ഫ��0~7��8��ͨ��������

    // 0
    seekfree_assistant_oscilloscope_data.dat[0] = encoder_left.encoder_filtered;
    // 1
    seekfree_assistant_oscilloscope_data.dat[1] = encoder_right.encoder_filtered;
    // 2
    seekfree_assistant_oscilloscope_data.dat[2] = speed.target_left; // direction_output;
    // 3
    seekfree_assistant_oscilloscope_data.dat[3] = speed.target_right; // direction.KP;
    // 4
    seekfree_assistant_oscilloscope_data.dat[4] = direction.KP;
    // 5
    seekfree_assistant_oscilloscope_data.dat[5] = direction_output;
    // 6
    seekfree_assistant_oscilloscope_data.dat[6] = direction.KF;
    // 7
    seekfree_assistant_oscilloscope_data.dat[7] = position;

    // // 4
    // seekfree_assistant_oscilloscope_data.dat[4] = motor_left.KP;
    // // 5
    // seekfree_assistant_oscilloscope_data.dat[5] = motor_left.KI;
    // // 6
    // seekfree_assistant_oscilloscope_data.dat[6] = motor_right.KP;
    // // 7
    // seekfree_assistant_oscilloscope_data.dat[7] = motor_right.KI;

    // // 4
    // seekfree_assistant_oscilloscope_data.dat[4] = inductor[LEFT_V];
    // // 5
    // seekfree_assistant_oscilloscope_data.dat[5] = inductor[RIGHT_V];
    // // 6
    // seekfree_assistant_oscilloscope_data.dat[6] = inductor[LEFT_H];
    // // 7
    // seekfree_assistant_oscilloscope_data.dat[7] = inductor[RIGHT_H];

    // ͨ������ת���ڷ�������ʾ��������
    seekfree_assistant_oscilloscope_send(&seekfree_assistant_oscilloscope_data);
}