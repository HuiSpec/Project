#ifndef __NBIOT_USART_H
#define __NBIOT_USART_H

#include "Delay.h"
#include "stdarg.h"
#include "stm32f10x.h"
#include "string.h"
#include <stdbool.h>
#include <stdio.h>
//串口1
#define USART1_GPIO_PORT GPIOA
#define USART1_GPIO_CLK RCC_APB2Periph_GPIOA
#define USART1_TX_GPIO_PIN GPIO_Pin_9
#define USART1_RX_GPIO_PIN GPIO_Pin_10

extern char USART1_RxPacket[];
extern char CSQ_Packet[10];
extern char IP_Packet[10];
extern char CIMI_Packet[100]; //定义接收数据包数组CIMI
extern char CEREG_Packet[10];
extern uint8_t USART1_RxFlag;

void USART1_Init(void);
void USART1_SendByte(uint8_t Byte);
void USART1_SendArray(uint8_t* Array, uint16_t Length);
void USART1_SendString(char* String);
void USART1_SendNumber(uint32_t Number, uint8_t Length);
void USART1_Printf(char* format, ...);
void USART1_SET(FunctionalState temp);

#endif /* __USART_H */
