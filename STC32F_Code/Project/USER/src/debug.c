#include "debug.h"

char send_str[32] = {0};
uint8 print_send_flag = 0;

void Wireless_Debug_Init(void)
{
    // 初始化无线转串口
    wireless_uart_init();

    // 设置函数指针
    seekfree_assistant_receive = wireless_uart_read_buff;

    // 设置函数指针 Debug_Parameter_Oscilloscope_Send()
    seekfree_assistant_transfer = seekfree_assistant_transfer_callback;
    // 需要传输通道数
    seekfree_assistant_oscilloscope_data.channel_num = 8;

    seekfree_assistant_init();
}

void Wireless_Seekfree_Assistant_Debug(void)
{
    uint8 i = 0;

    // 助飞助手解析接收到的数据
    seekfree_assistant_data_analysis();

    // 遍历
    for (i = 0; i < SEEKFREE_ASSISTANT_SET_PARAMETR_COUNT; i++)
    {
        // 更新标志位
        if (seekfree_assistant_parameter_update_flag[i])
        {
            seekfree_assistant_parameter_update_flag[i] = 0;

            // 发送通道信息
            sprintf(send_str, "\r\nreceive data channel : %d  ", i);
            wireless_uart_send_buff((uint8 *)send_str, strlen(send_str));

            // 清空之前存储的字符串
            memset(send_str, 0, strlen(send_str));

            // 将接收到的数据赋值给对应变量
            Debug_Parameter_Assignment(i);

            // 发送接收到的数据
            func_float_to_str(send_str, seekfree_assistant_parameter[i], 2);
            wireless_uart_send_buff("data : ", strlen("data : "));
            wireless_uart_send_buff((uint8 *)send_str, strlen(send_str));

            // 清空之前存储的字符串
            memset(send_str, 0, strlen(send_str));
        }
    }
}

/**
 * @description:
 * @param {uint8} i 对应逐飞助手中0~7共8个通道
 * @return {*}
 */
void Debug_Parameter_Assignment(uint8 i)
{
    switch (i)
    {
    case 0:
        speed.normal = seekfree_assistant_parameter[i];
        break;
    case 1:
        speed.deceleration_factor = seekfree_assistant_parameter[i];
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
        direction.KP_2 = seekfree_assistant_parameter[i];
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
// 函数简介     逐飞助手发送函数
// 参数说明     *buff           需要发送的数据地址
// 参数说明     length          需要发送的长度
// 返回参数     uint32          剩余未发送数据长度
// 使用示例
//-------------------------------------------------------------------------------------------------------------------
uint32 seekfree_assistant_transfer_callback(const uint8 *buff, uint32 length)
{
    uart_putbuff(WIRELESS_UART, buff, length);
    return 0;
}

/**
 * @note: 数据通道的数量由seekfree_assistant_oscilloscope_data.channel_num决定，其在Wireless_Debug_Init()中被赋值
 * @return {*}
 */
void Debug_Parameter_Oscilloscope_Send(void)
{
    // 设置数据
    seekfree_assistant_oscilloscope_data.dat[0] = encoder_left.encoder_filtered;
    seekfree_assistant_oscilloscope_data.dat[1] = encoder_right.encoder_filtered;
    seekfree_assistant_oscilloscope_data.dat[2] = direction_output;
    seekfree_assistant_oscilloscope_data.dat[3] = inductor[LEFT_V];
    seekfree_assistant_oscilloscope_data.dat[4] = inductor[RIGHT_V];
    seekfree_assistant_oscilloscope_data.dat[5] = inductor[LEFT_H];
    seekfree_assistant_oscilloscope_data.dat[6] = inductor[RIGHT_H];
    seekfree_assistant_oscilloscope_data.dat[7] = direction.KP;

    // 通过无线转串口发送虚拟示波器数据
    seekfree_assistant_oscilloscope_send(&seekfree_assistant_oscilloscope_data);
}