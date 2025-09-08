/**
 * @file main.c
 * @brief
 * @author HuiSpec (liwenhuiyx@yeah.net)
 * @version 1.0
 * @date 2025-03-18
 *
 * @copyright Copyright (c) 2025  58
 *
 */

#include "Delay.h"

#include "GPS.h"
// #include "NBIOT_Usart.h"
// #include "GPS_Usart.h"
// #include "NBIOT_Usart.h"
// #include "GPS_Usart.h"
#include "LED.h"
#include "NBIOT_Usart.h"
#include "OLED.h"
#include "OneWrite.h"
#include "Usart.h"

#include "nbiot.h"
#include "stm32f10x.h" // Device header
#include "string.h"
#include <stdio.h>

// #include <cstdio>
// #include <cstdio>

char DATA[500]      = {0};
char SEND_DATA[500] = {0};
float temp;

int main()
{
    //     // /*模块初始化*/
    OLED_Init(); // OLED初始化
    //     // Onewrite_Init();
    // Usart3_Init();
    // GPS_get();
    Usart3_Init();
    // nbiot_Init();
    nbiot_init();
    printf("NBIOT_INIT\r\n");
    GPS_init();
    printf("GPS_INIT\r\n");

    Onewrite_Init();
    printf("ONEWRITE_INIT\r\n");

    while (1)
    {
        GPS_get();
        printf("GPS_OPEN\r\n");

        Delay_ms(500);
        printf("GPS_GET\r\n");
        OLED_ShowString(1, 1, IP_Packet);

        temp = Onewrite_Get_Temp();
        sprintf(DATA, "%s,%s,%s,$TEMP:%.2f", IP_Packet, GNZDA_Packet, GNGGA_Packet, temp);

        GPS_close();
        printf("GPS_close\r\n");
        // OLED_ShowString(1, 1, "-------");
        // printf("---------------------");
        // printf("\r\n");
        // printf("AT+QISEND=0,%d,\"%s\"", strlen(DATA), DATA);
        sprintf(SEND_DATA, "AT+QISEND=0,%d,\"%s\"", strlen(DATA), DATA);
        // printf("\r\n");
        printf("%s", SEND_DATA);

        NB_IoT_conect_send(SEND_DATA);
        OLED_ShowHexNum(2, 1, ERROR_FLAG, 4);
        // temp = Onewrite_Get_Temp();
        // OLED_ShowString(1, 1, USART2_RxPacket);
        // OLED_ShowString(2, 1, GNGGA_Packet);
        // OLED_ShowString(3, 1, GNGLL_Packet);
        // OLED_ShowString(4, 1, GNZDA_Packet);
        // USART2_SendString("-------");
        // OLED_ShowString(2, 1, "-------");
        // OLED_ShowNum(2, 1, temp, 5);
        // printf("TEMP:%.4f", temp);
        // printf("\r\n");

        // Delay_ms(500);
        // GPS_close();
        // Usart3_SendString("000000");
        // Usart3_SendString("\r\n");
        // Usart3_SendNumber(temp,4);
        // Usart3_SendString("\r\n");

        // Usart3_SendString(USART2_RxPacket);
        // Usart3_SendString("\r\n");

        // Usart3_SendString(GNGGA_Packet);
        // Usart3_SendString("\r\n");

        // Usart3_SendString(GNGLL_Packet);
        // Usart3_SendString("\r\n");

        // Usart3_SendString(GNZDA_Packet);
        // Usart3_SendString("\r\n");

        // printf("GNGGA:%s,GNGLL:%s,GNZDA:%s", GNGGA_Packet, GNGLL_Packet, GNZDA_Packet);
        Delay_s(3);
    }
    //     // Delay_s(1);
    //     // NBIOT_send_command(AT_NB_clk, "OK", 5);
    //     nbiot_Init();
    //     Delay_ms(500);
    //     // NBIOT_send_command(AT_NB_tcp_close,"OK",1);
    //     // nbiot_connect();
    //     // LED_Init(); // LED初始化
    //     // Serial_Init(); //串口初始化
    //     // // LED1_ON();
    //     // // Serial_SendString("AT");
    //     // /*显示静态字符串*/
    //     // OLED_ShowString(1, 1, "TxPacket");
    //     // OLED_ShowString(3, 1, "RxPacket");
    //     while (1)
    //     {
    //         // USART1_SendString(AT_NB_cereg);
    //         // USART1_SendString(AT_NB_csq);
    //         // USART1_SendString(AT_NB_cimi);
    //         // OLED_ShowString(1, 1, CSQ_Packet);
    //         // OLED_ShowString(2, 1, CIMI_Packet);
    //         // OLED_ShowString(3, 1, CEREG_Packet);
    //         sprintf(DATA, "SN:%s,TIME:%s,NE:%s", SN, GNZDA_Packet, GNGGA_Packet);
    //         sprintf(SEND_DATA, "AT+QISEND=0,%d,%s", sizeof(DATA), DATA);
    //         OLED_ShowString(2, 1, USART1_RxPacket);
    //         // OLED_ShowHexNum(1, 1, ERROR_FLAG, 3);
    //         // nbiot_send_data("AT+QISEND=0,10,\"1234567890\"");

    //         // nbiot_send_data("AT+QISEND=0,10,\"1234567890\"");

    //         // Delay_s(1);
    //         // nbiot_get_cimi();
    //         // OLED_ShowString(3, 1, CIMI_Packet);
    //         // NBIOT_send_command("AT+QISEND=0,10,\"1234567890\"", "OK", 3);
    //         Delay_s(5);
    //         // OLED_ShowString(3, 1, GNGLL_Packet);
    //         // OLED_ShowString(4, 1, GNZDA_Packet);
    //         // Serial_SendString(GNGGA_Packet);
    //         // Serial_SendString(GNGLL_Packet);
    //         // Serial_SendString(GNZDA_Packet);
    //         // Serial_SendString("---------------------------");
    //         // Delay_s(2);

    //         // LED1_Turn();
    //         // USART_Send_String(USART2, "Led Close Successful\n");
    //         // Delay_ms(500);

    //         // nbiot_send_at();
    //         // if (Serial_RxFlag == 1)
    //         // {
    //         //     if (strcmp(Serial_RxPacket, "OK") == 0)
    //         //     {
    //         //         LED1_Turn();
    //         //         OLED_ShowString(4, 1, Serial_RxPacket);
    //         //     }
    //         //     else if (strcmp(Serial_RxPacket, "NO") == 0)
    //         //     {
    //         //         LED1_OFF();
    //         //         OLED_ShowString(2, 1, Serial_RxPacket);
    //         //     }
    //         // }
    //         // while (NBIOT_send_command(AT_NB_clk, "OK", 3))
    //         // {
    //         //     // if (ERROR_FLAG)
    //         //     // {
    //         //     //     break;
    //         //     // }
    //         //     LED1_Turn();
    //         //     Delay_s(2);
    //         // }
    //         // OLED_ShowString(1, 1, "--------");
    //         // if (Serial_RxFlag == 1)
    //         // {
    //         //     Serial_SendString(GNGGA_Packet);
    //         //     Serial_SendString(GNGLL_Packet);
    //         //     Serial_SendString(GNZDA_Packet);
    //         //     Serial_RxFlag = 0;
    //         // }
    //         // if (NBIOT_RxFlag == 1) //如果接收到数据包
    //         // {
    //         //     // OLED_ShowString(4, 1, "                ");
    //         //     // OLED_ShowString(4, 1, Serial_RxPacket); //
    //         OLED清除指定位置，并显示接收到的数据包

    //         //     /*将收到的数据包与预设的指令对比，以此决定将要执行的操作*/
    //         //     if (strcmp(NBIOT_RxPacket, "LED_ON") == 0) //如果收到LED_ON指令
    //         //     {
    //         //         LED1_ON();                         //点亮LED
    //         //         NBIOT_SendString("LED_ON_OK\r\n"); //串口回传一个字符串LED_ON_OK
    //         //         // OLED_ShowString(2, 1, "                ");
    //         //         // OLED_ShowString(2, 1, "LED_ON_OK"); //
    //         OLED清除指定位置，并显示LED_ON_OK
    //         //     }
    //         //     else if (strcmp(NBIOT_RxPacket, "AT_NB_reboot") == 0) //如果收到LED_OFF指令
    //         //     {
    //         //         LED1_OFF();                         //熄灭LED
    //         //         NBIOT_SendString("LED_OFF_OK\r\n"); //串口回传一个字符串LED_OFF_OK
    //         //         // OLED_ShowString(2, 1, "                ");
    //         //         // OLED_ShowString(2, 1, "LED_OFF_OK"); //
    //         OLED清除指定位置，并显示LED_OFF_OK
    //         //     }
    //         //     else //上述所有条件均不满足，即收到了未知指令
    //         //     {
    //         //         NBIOT_SendString("ERROR_COMMAND\r\n"); //串口回传一个字符串ERROR_COMMAND
    //         //         // OLED_ShowString(2, 1, "                ");
    //         //         // OLED_ShowString(2, 1, "ERROR_COMMAND"); //
    //         //         /// OLED清除指定位置，并显示ERROR_COMMAND
    //         //     }

    //         //     NBIOT_RxFlag = 0;
    //         //     //处理完成后，需要将接收数据包标志位清零，否则将无法接收后续数据包
    //         //     // }
    //         // }
    //     }
}
//     // // void TIM2_IRQHandler()
//     // // {
//     // //     if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
//     // //     {
//     // //         number1++;
//     // //         TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//     // //     }
//     // // }

// #include "Delay.h"
// #include "OneWrite.h"
// #include "Usart.h"
// #include "stm32f10x.h"
// #include "string.h" //strlen、memset用到
// #include <stdio.h>

// extern unsigned char Onewrite_ID[8][8]; //检测到的传感器ID存数组
// extern unsigned char Onewrite_SensorNum;

// int main(void)
// {
//     u8 num = 0;
//     Usart_Init();
//     while (Onewrite_Init()) //初始化Onewrite_Init(),兼检测18B20
//     {
//         printf("Onewrite_Init() Check Failed!\r\n");
//     }
//     printf("Onewrite_Init() Ready!\r\n");
//     while (1)
//     {
//         // Onewrite_Get_Temp();const char *restrict __format, ...
//         printf("TM:%.2f\r\n", Onewrite_Get_Temp());
//         // Onewrite_Search_Rom();
//         // Serial_SendString("Onewrite\r\n");
//         // printf("Onewrite_Init()_SensorNum:%d\r\n", Onewrite_SensorNum);
//         // for (num = 0; num < Onewrite_SensorNum; num++)
//         // {
//         //     printf("ID:%02x%02x%02x%02x%02x%02x%02x%02x TM:%.2f\r\n", Onewrite_ID[num][0],
//         //            Onewrite_ID[num][1], Onewrite_ID[num][2], Onewrite_ID[num][3],
//         //            Onewrite_ID[num][4], Onewrite_ID[num][5], Onewrite_ID[num][6],
//         //            Onewrite_ID[num][7], Onewrite_Get_Temp(num));
//         // }
//         // printf("\r\n");
//         Delay_s(2);
//     }
// }