#ifndef __ONEWRITE_H
#define __ONEWRITE_H

#include "Delay.h"
#include "stm32f10x.h"

#define Onewrite_GPIO_NUM GPIO_Pin_1
#define Onewrite_GPIO_X GPIOB
#define RCC_APB2Periph_Onewrite_GPIO_X RCC_APB2Periph_GPIOB

#define Onewrite_DQ_OUT_Low GPIO_ResetBits(Onewrite_GPIO_X, Onewrite_GPIO_NUM)
#define Onewrite_DQ_OUT_High GPIO_SetBits(Onewrite_GPIO_X, Onewrite_GPIO_NUM)
#define Onewrite_DQ_IN GPIO_ReadInputDataBit(Onewrite_GPIO_X, Onewrite_GPIO_NUM)

u8 Onewrite_Init(void);
u8 Onewrite_Read_Byte(void);
u8 Onewrite_Read_Bit(void);
u8 Onewrite_Answer_Check(void);
void Onewrite_GPIO_Config(void);
void Onewrite_Mode_IPU(void);
void Onewrite_Mode_Out(void);
void Onewrite_Rst(void);
void Onewrite_Search_Rom(void);
void Onewrite_Write_Byte(u8 dat);
float Onewrite_Get_Temp();

#endif