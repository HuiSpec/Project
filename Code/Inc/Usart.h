#ifndef __USART_H
#define __USART_H

#include "stdarg.h"
#include "stm32f10x.h"
#include <stdio.h>
//串口1
#define USART3_GPIO_PORT GPIOB
#define USART3_GPIO_CLK RCC_APB2Periph_GPIOB
#define USART3_TX_GPIO_PIN GPIO_Pin_10
#define USART3_RX_GPIO_PIN GPIO_Pin_11

extern char Usart3_RxPacket[];
extern uint8_t Usart3_RxFlag;

void Usart3_Init(void);
void Usart3_SendByte(uint8_t Byte);
void Usart3_SendArray(uint8_t* Array, uint16_t Length);
void Usart3_SendString(char* String);
void Usart3_SendNumber(uint32_t Number, uint8_t Length);
void Usart3_Printf(char* format, ...);
void Usart3_SET(FunctionalState temp);

#endif /* __USART3_H */
