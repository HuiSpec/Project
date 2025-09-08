/*
 * @Author: LiWenhui liwenhuiyx@yeah.net
 * @Date: 2024-08-18 17:01:06
 * @LastEditors: LiWenhui liwenhuiyx@yeah.net
 * @LastEditTime: 2024-08-18 17:42:35
 * @FilePath: /005_输入捕获/Code/Inc/OLED.h
 *
 */
#ifndef __OLED_H
#define __OLED_H

#include "stdint.h"
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char* String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

#endif