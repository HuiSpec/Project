#include "nbiot.h"
#include "Delay.h"
#include "NBIOT_Usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

uint8_t ERROR_FLAG  = 0x00;
uint8_t send_number = 10;

// bool NBIOT_send_command(char* command, char* ans, uint8_t num)
// {
//     // static uint8_t count = 0;

//     // Delay_ms(1);
//     // count++;

//     USART1_SendString(command);
//     if (USART1_RxFlag == 1)
//     {
//         if (strcmp(USART1_RxPacket, ans) == 0)
//         {
//             USART1_RxFlag = 0;
//             // ERROR_FLAG++;
//             return true;
//         }
//         return false;
//     }
//     Delay_s(1);
//     ERROR_FLAG = 1;
//     return false;
// }

void NBIOT_restart_init()
{
    RCC_APB2PeriphClockCmd(nbiot_restart_clk, ENABLE);
    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = nbiot_restart_pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(nbiot_restart_port, &GPIO_InitStructure); //将PA11引脚初始化为推挽输出
}

void nbiot_restart()
{
    NBIOT_restart_init();
    GPIO_ResetBits(nbiot_restart_port, nbiot_restart_pin);
    Delay_s(1);
    GPIO_SetBits(nbiot_restart_port, nbiot_restart_pin);
}
// // //禁⽤休眠模式
// // AT+QSCLK=0
// // //获取信号质量
// // AT+CSQ
// // //获取注⽹状态，0,1表⽰注⽹成功
// // AT+CEREG?
// // //获取卡号
// // AT+CIMI
// // //重启模组
// // AT+QRST=1
// // 5. UDP
// // ✅ //设置模式
// // AT+QICFG="dataformat",0,0
// // OK
// // //连接服务器
// // AT+QIOPEN=0,0,"UDP","123.207.210.43",9999
// // OK
// // +QIOPEN:0,0
// // //发送数据命令
// // AT+QISEND=0,10,"1234567890"
// // OK
// // SENDOK
// // 野牛物联网
// // //⾃动接收到数据
// // +QIURC:"recv",0,10,"1234567890"
// // //关闭UDP接⼝命令
// // AT+QICLOSE=0
// // OK
// // CLOSEOK
// // void nbiot_Init()
// // {
// //     // Delay_s(1);
// //     NBIOT_send_command(AT_NB_clk, "OK", send_number);
// //     NBIOT_send_command(AT_NB_at, "OK", send_number);
// // }

// // void nbiot_connect(void)
// // {
// //     NBIOT_send_command(AT_NB_tcp_data_formate, "OK", send_number);
// //     NBIOT_send_command(AT_NB_tcp_connect, "OK", send_number);
// // }
// void nbiot_Init()
// {
//     nbiot_restart();
//     USART1_Init();
//     while (strstr(USART1_RxPacket, "+IP") == NULL) {}
//     // USART1_SendString(AT_NB_clk);
//     // NBIOT_send_command(AT_NB_clk, "OK", 5);
//     // if (!NBIOT_send_command(AT_NB_clk, "OK", send_number))
//     // {
//     //     ERROR_FLAG |= 0x01; // 设置错误标志
//     //     return;
//     // }
//     bool success = false;
//     while (!success && send_number--)
//     {
//         success = NBIOT_send_command(AT_NB_clk, "OK", 1);
//         if (!success)
//         {
//             ERROR_FLAG |= 0x01; // 设置错误标志
//             Delay_s(1);         // 等待一段时间再重试
//         }
//     }
//     if (!success)
//     {
//         return; // 所有尝试都失败后返回
//     }

//     if (!NBIOT_send_command(AT_NB_at, "OK", send_number))
//     {
//         ERROR_FLAG |= 0x02; // 设置错误标志
//         return;
//     }
//     if (!NBIOT_send_command(AT_NB_clk, "OK", send_number))
//     {
//         ERROR_FLAG |= 0x04; // 设置错误标志
//         return;
//     }
//     // while (send_number--)
//     // {
//     //     USART1_SendString(AT_NB_clk);
//     //     if (USART1_RxFlag == 1)
//     //     {
//     //         if (strcmp(USART1_RxPacket, "OK") == 0)
//     //         {
//     //             USART1_RxFlag = 0;
//     //             // ERROR_FLAG++;
//     //             return;
//     //         }
//     //         // return false;
//     //     }
//     //     Delay_s(1);
//     // }
//     // while (strstr(USART1_RxPacket, "OK") == NULL)
//     // {
//     //     // USART1_SendString(AT_NB_clk);
//     // }
//     // Delay_ms(100);
//     // if (!NBIOT_send_command(AT_NB_clk, "OK", send_number))
//     // {
//     //     ERROR_FLAG |= 0x01; // 设置错误标志
//     //     return;
//     // }

//     // {
//     //     return;
//     // }

//     // 禁用休眠模式

//     // 获取信号质量
//     // if (!NBIOT_send_command("AT+CSQ\r\n", "OK", send_number))
//     // {
//     //     ERROR_FLAG |= 0x02; // 设置错误标志
//     //     return;
//     // }

//     // 获取注册状态
//     // if (!NBIOT_send_command(AT_NB_ready, "OK", send_number))
//     // {
//     //     ERROR_FLAG |= 0x02; // 设置错误标志
//     //     return;
//     // }

//     // 获取卡号
//     // if (!NBIOT_send_command("AT+CIMI\r\n", "OK", send_number))
//     // {
//     //     ERROR_FLAG |= 0x08; // 设置错误标志
//     //     return;
//     // }

//     // 重启模组
//     // if (!NBIOT_send_command("AT+QRST=1\r\n", "OK", send_number))
//     // {
//     //     ERROR_FLAG |= 0x10; // 设置错误标志
//     //     return;
//     // }

//     // 设置模式
// }

// // void nbiot_get_cimi()
// // {
// //     USART1_SendString(AT_NB_cimi);
// //     if (USART1_RxFlag == 1)
// //     {
// //         // if (USART1_RxPacket[0] < '9' && USART1_RxPacket[0] > '0')
// //         // {
// //         // }
// //         strcpy(CIMI_Packet, USART1_RxPacket);

// //         USART1_RxFlag = 0;
// //     }
// // }
// // 连接NB-IoT服务器
// void nbiot_send_data(char* data)

// {
//     ERROR_FLAG &= 0x01;
//     // if (!NBIOT_send_command(AT_NB_tcp_data_formate, "OK", send_number))
//     // {
//     //     ERROR_FLAG |= 0x10; // 设置错误标志
//     //     return;
//     // }
//     // Delay_ms(100);
//     // 连接服务器
//     // if (!NBIOT_send_command(AT_NB_tcp_connect, "OK", send_number))
//     // {
//     //     ERROR_FLAG |= 0x20; // 设置错误标志
//     //     return;
//     // }
//     // while (send_number--)
//     // {
//     //     USART1_SendString(AT_NB_tcp_connect);
//     //     if (USART1_RxFlag == 1)
//     //     {
//     //         if (strcmp(USART1_RxPacket, "OK") == 0)
//     //         {
//     //             USART1_RxFlag = 0;
//     //             // ERROR_FLAG++;
//     //             USART1_SendString(data);
//     //             Delay_s(1);
//     //             USART1_SendString(AT_NB_tcp_close);
//     //             return;
//     //         }
//     //         // return false;
//     //     }
//     //     Delay_s(1);
//     // }
//     if (!NBIOT_send_command(AT_NB_tcp_connect, "OK", send_number))
//     {
//         ERROR_FLAG |= 0x10; // 设置错误标志
//         return;
//     }
//     if (!NBIOT_send_command(data, "OK", send_number))
//     {
//         ERROR_FLAG |= 0x20; // 设置错误标志
//         return;
//     }
//     if (!NBIOT_send_command(AT_NB_tcp_close, "OK", send_number))
//     {
//         ERROR_FLAG |= 0x40; // 设置错误标志
//         return;
//     }
//     // Delay_s(2);

//     // if (!NBIOT_send_command(data, "OK", send_number))
//     // {
//     //     // ERROR_FLAG |= 0x40; // 设置错误标志
//     //     return;
//     // }

//     // Delay_s(1);

//     // // if (!NBIOT_send_command(AT_NB_tcp_close, "OK", send_number))
//     // // {
//     // //     ERROR_FLAG |= 0x80; // 设置错误标志
//     // //     return;
//     // // }
//     // USART1_SendString(AT_NB_tcp_close);
//     // Delay_s(1);

//     // 等待连接成功响应
//     // if (!NBIOT_send_command("+QIOPEN:", "+QIOPEN: 0,0", send_number))
//     // {
//     //     ERROR_FLAG |= 0x40; // 设置错误标志
//     //     return;
//     // }

//     // 发送数据
//     // if (!NBIOT_send_command("AT+QISEND=0,10,\"1234567890\"\r\n", "SEND OK", send_number))
//     // {
//     //     ERROR_FLAG |= 0x100; // 设置错误标志
//     //     return;
//     // }

//     // // 等待接收数据
//     // if (!NBIOT_send_command("+QIURC: \"recv\",0,10,\"1234567890\"",
//     //                         "+QIURC: \"recv\",0,10,\"1234567890\"", send_number))
//     // {
//     //     ERROR_FLAG |= 0x200; // 设置错误标志
//     //     return;
//     // }

//     // 关闭UDP接口
//     // if (!NBIOT_send_command("AT+QICLOSE=0\r\n", "CLOSE OK", send_number))
//     // {
//     //     ERROR_FLAG |= 0x400; // 设置错误标志
//     //     return;
//     // }
// }

// void USART1_SendString(char* cmd)
// {

//     while (*cmd != '\0')
//     {
//         USART_SendData(USART1, *cmd++);
//         while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == Bit_RESET)
//             ; //发送数据结束
//     }
// }

u8 NB_IoT_ack_chack(char* str)
{
    Delay_ms(10);
    if (USART1_RxFlag != 0)
    {
        USART1_RxFlag = 0;

        if (strstr((const char*)USART1_RxPacket, (const char*)str)) //符合预期
        {
            memset(USART1_RxPacket, 0, 100);
            return 0;
        }
        else //不符合预期
        {
            memset(USART1_RxPacket, 0, 100);
            return 1;
        }
    }
    else
    {
        memset(USART1_RxPacket, 0, 100); //清空数组
        return 1;
    }
}

u8 NB_IotConect()
{
    u8 x = 1;
// cmd1:
//     USART1_SendString(AT_NB_at); //  指令测试
//     if (!NB_IoT_ack_chack("OK"))
//         x = 0;
//     else
//         goto cmd1;
cmd1:
    USART1_SendString(AT_NB_clk);
    Delay_ms(500);
    if (!NB_IoT_ack_chack("OK"))
        x = 0;
    else
        goto cmd1;
    return x;
}

u8 NB_IoT_conect_send(char* data)
{
    u8 x = 0;

cmd2:
    USART1_SendString(AT_NB_tcp_data_formate);
    Delay_ms(500);

    if (!NB_IoT_ack_chack("OK"))
        x++;
    else
        goto cmd2;
cmd3:
    USART1_SendString(AT_NB_tcp_connect);
    Delay_ms(500);

    if (!NB_IoT_ack_chack("OK"))
        x++;
    else
        goto cmd3;
cmd4:
    USART1_SendString(data);
    Delay_ms(500);

    if (!NB_IoT_ack_chack("OK"))
        x++;
    else
        goto cmd4;
cmd5:
    USART1_SendString(AT_NB_tcp_close);
    Delay_ms(500);

    if (!NB_IoT_ack_chack("OK"))
        x++;
    else
        goto cmd5;

    if (x == 4)
        return 0; // COAP连接成功
    else
        return 1; // COAP连接失败
}

void nbiot_init()
{
    nbiot_restart();
    USART1_Init();
    while (strstr(USART1_RxPacket, "+IP") == NULL) {}
    if (NB_IotConect() != 0)
    {
        ERROR_FLAG |= 0x01;
        return;
    }
    // NB_IotConect();
}