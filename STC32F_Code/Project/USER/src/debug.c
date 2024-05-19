#include "debug.h"

#define TEMP_BUFFER_SIZE 64
static uint8 temp_uart_buffer[TEMP_BUFFER_SIZE]; // 数据存放数组

char send_str[32] = {0};

void Wireless_Debug_Init(void)
{
    // 初始化无线转串口
    wireless_uart_init();

    // 设置函数指针
    seekfree_assistant_receive = wireless_uart_read_buff;

    seekfree_assistant_init();
}

void Wireless_Debug_Main(void)
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
