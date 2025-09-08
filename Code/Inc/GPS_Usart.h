#ifndef __GPS_USART_H
#define __GPS_USART_H

#include "stdio.h"
#include "stm32f10x.h"
#include "string.h"
//串口122222222222222222222222222222222222222222222
#define USART2_GPIO_PORT GPIOA
#define USART2_GPIO_CLK RCC_APB2Periph_GPIOA
#define USART2_TX_GPIO_PIN GPIO_Pin_2
#define USART2_RX_GPIO_PIN GPIO_Pin_3

extern char USART2_RxPacket[];
extern uint8_t USART2_RxFlag;
extern char GNGGA_Packet[100];
extern char GNGLL_Packet[100];
extern char GNZDA_Packet[100];

void USART2_Init(void);
void USART2_SendByte(uint8_t Byte);
void USART2_SendArray(uint8_t* Array, uint16_t Length);
void USART2_SendString(char* String);
void USART2_SendNumber(uint32_t Number, uint8_t Length);
// void GPS_Printf(char* format, ...);
void USART2_SET(FunctionalState temp);
#endif /* __USART_H */
