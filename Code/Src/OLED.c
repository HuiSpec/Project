/*
 * @Author: LiWenhui liwenhuiyx@yeah.net
 * @Date: 2024-08-17 13:00:27
 * @LastEditors: LiWenhui liwenhuiyx@yeah.net
 * @LastEditTime: 2024-08-17 13:46:14
 * @FilePath: /002_/Code/Src/OLED.c
 *
 */
/**
 * @brief 显示codetab.h中的ASCII字符,有 6*8 和 8*16
 * @param Line                   : 显示在第几行  x∈[0～4]
 * @param Column                 : 显示在第几列 y ∈ [0~16]
 * @param Number|char|Str        : 要显示的内容
 * @param Length                 : 要显示的数字长度
 * @param TextSize               : 字符大小（默认为8*16--Size=2）
 * @retval
 *
 */
#include "I2COLED.h"

#define SIZE 2
#define X (8 * Column - 8)
#define Y (2 * Line - 2)

// 初始化OLED显示
void OLED_Init(void)
{
    I2COLED_Init();
}

// 清除OLED显示内容
void OLED_Clear(void)
{
    I2COLED_CLS();
}

// 在指定行和列显示一个字符
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{
    I2COLED_ShowChar(X, Y, Char, SIZE);
}

// 在指定行和列显示一个字符串
void OLED_ShowString(uint8_t Line, uint8_t Column, char* String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++)
    {
        OLED_ShowChar(Line, Column + i, String[i]);
    }
}

// 在指定行和列显示一个无符号整数
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    I2COLED_ShowNum(X, Y, Number, Length, SIZE);
}

// 在指定行和列显示一个有符号整数
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
    I2COLED_ShowSignedNum(X, Y, Number, Length, SIZE);
}

// 在指定行和列显示一个十六进制数
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    I2COLED_ShowHexNum(X, Y, Number, Length, SIZE);
}

// 在指定行和列显示一个二进制数
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    I2COLED_ShowBinNum(X, Y, Number, Length, SIZE);
}