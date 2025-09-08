/*
 * @Author: LiWenhui liwenhuiyx@yeah.net
 * @Date: 2024-08-13 19:48:34
 * @LastEditors: LiWenhui liwenhuiyx@yeah.net
 * @LastEditTime: 2024-08-17 13:25:35
 * @FilePath: /002_/Code/Inc/I2COLED.h
 *
 */
#ifndef __I2COLED_H
#define __I2COLED_H

#include "stm32f10x.h"

#define I2COLED_ADDRESS 0x78 // 通过调整OR电阻，屏可以 0x78和0x7A两个地址，默认0x78
uint32_t I2COLED_Pow(uint32_t X, uint32_t Y);
void I2C_Configuration(void);
void I2C_WriteByte(uint8_t addr, uint8_t data);
void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data);
void I2COLED_Init(void);
void I2COLED_SetPos(unsigned char x, unsigned char y);
void I2COLED_Fill(unsigned char fill_Data);
void I2COLED_CLS(void);
void I2COLED_ON(void);
void I2COLED_OFF(void);

void I2COLED_ShowChar(uint8_t x, uint8_t y, char Char, unsigned char TextSize);
void I2COLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void I2COLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void I2COLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1,
                     unsigned char BMP[]);
void I2COLED_ShowNum(unsigned char x, unsigned char y, uint32_t Num, uint8_t Length,
                     unsigned char TextSize);
void I2COLED_ShowSignedNum(unsigned char x, unsigned char y, int32_t Num, uint8_t Length,
                           unsigned char TextSize);
void I2COLED_ShowHexNum(uint8_t x, uint8_t y, uint32_t Num, uint8_t Length, unsigned char TextSize);
void I2COLED_ShowBinNum(uint8_t x, uint8_t y, uint32_t Num, uint8_t Length, unsigned char TextSize);

#endif