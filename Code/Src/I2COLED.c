/*
 * @Author: LiWenhui liwenhuiyx@yeah.net
 * @Date: 2024-08-17 13:16:06
 * @LastEditors: LiWenhui liwenhuiyx@yeah.net
 * @LastEditTime: 2024-08-17 13:24:35
 * @FilePath: /002_/Code/Src/I2COLED.c
 *
 */
/*
 * Others: none;
 *
 * Function List:
 * 1. void I2C_Configuration(void) -- 配置CPU的硬件I2C
 * 2. void I2C_WriteByte(uint8_t addr,uint8_t data) -- 向寄存器地址写一个byte的数据
 * 3. void WriteCmd(unsigned char I2C_Command) -- 写命令
 * 4. void WriteDat(unsigned char I2C_Data) -- 写数据
 * 5. void I2COLED_Init(void) -- OLED屏初始化
 * 6. void OLED_SetPos(unsigned char x, unsigned char y) -- 设置起始点坐标
 * 7. void OLED_Fill(unsigned char fill_Data) -- 全屏填充
 * 8. void OLED_CLS(void) -- 清屏
 * 9. void OLED_ON(void) -- 唤醒
 * 10. void OLED_OFF(void) -- 睡眠
 * 11. void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char
 *TextSize) -- 显示字符串(字体大小有8*16两种)6*8和
 * 12. void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N) --
 *显示中文(中文需要先取模，然后放到codetab.h中)
 * 13. void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char
 *y1,unsigned char BMP[]) -- BMP图片
 *
 * History: none;
 *
 *************************************************************************************/

#include "I2COLED.h"
#include "Codetab.h"
#include "Delay.h"

void I2C_Configuration(void)
{
    I2C_InitTypeDef I2C_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);  // 使用 APB1 的外设 I2C1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 使用 APB2 的外设 GPIOB

    /*STM32F103C8T6芯片的硬件I2C: PB6 -- SCL; PB7 -- SDA */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7; // 查 引脚图
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD; // I2C必须开漏输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_DeInit(I2C1); //使用I2C
    I2C_InitStructure.I2C_Mode                = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle           = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1         = 0x30; //主机的I2C地址，随便写的
    I2C_InitStructure.I2C_Ack                 = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed          = 400000; // 400K 时钟频率 <= 400KHz

    I2C_Cmd(I2C1, ENABLE);              // I2C 使能
    I2C_Init(I2C1, &I2C_InitStructure); // 初始化 I2C 结构体
}

void I2C_WriteByte(uint8_t addr, uint8_t data)
{
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
        ;

    I2C_GenerateSTART(I2C1, ENABLE); //开启 I2C1
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
        ; /*EV5,主模式*/

    I2C_Send7bitAddress(I2C1, I2COLED_ADDRESS, I2C_Direction_Transmitter); //器件地址 -- 默认0x78
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        ;

    I2C_SendData(I2C1, addr); // 寄存器地址
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;

    I2C_SendData(I2C1, data); //发送数据
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;

    I2C_GenerateSTOP(I2C1, ENABLE); //关闭I2C1总线
}

void WriteCmd(unsigned char I2C_Command) // 写命令
{
    I2C_WriteByte(0x00, I2C_Command);
}

void WriteDat(unsigned char I2C_Data) //写数据
{
    I2C_WriteByte(0x40, I2C_Data);
}

void I2COLED_Init(void)
{

    I2C_Configuration();
    Delay_ms(100); // 这里延时很重要

    WriteCmd(0xAE); // display off
    WriteCmd(0x20); // Set Memory Addressing Mode
    WriteCmd(0x10); // 00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing
                    // Mode (RESET);11,Invalid
    WriteCmd(0xb0); // Set Page Start Address for Page Addressing Mode,0-7
    WriteCmd(0xc8); // Set COM Output Scan Direction
    WriteCmd(0x00); //---set low column address
    WriteCmd(0x10); //---set high column address
    WriteCmd(0x40); //--set start line address
    WriteCmd(0x81); //--set contrast control register
    WriteCmd(0xff); //亮度调节 0x00~0xff
    WriteCmd(0xa1); //--set segment re-map 0 to 127
    WriteCmd(0xa6); //--set normal display
    WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
    WriteCmd(0x3F); //
    WriteCmd(0xa4); // 0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    WriteCmd(0xd3); //-set display offset
    WriteCmd(0x00); //-not offset
    WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
    WriteCmd(0xf0); //--set divide ratio
    WriteCmd(0xd9); //--set pre-charge period
    WriteCmd(0x22); //
    WriteCmd(0xda); //--set com pins hardware configuration
    WriteCmd(0x12);
    WriteCmd(0xdb); //--set vcomh
    WriteCmd(0x20); // 0x20,0.77xVcc
    WriteCmd(0x8d); //--set DC-DC enable
    WriteCmd(0x14); //
    WriteCmd(0xaf); //--turn on I2COLED panel
    I2COLED_Fill(0xff);
    I2COLED_Fill(0x00);
}

void I2COLED_SetPos(unsigned char x, unsigned char y) //设置起始点坐标
{
    WriteCmd(0xb0 + y);
    WriteCmd(((x & 0xf0) >> 4) | 0x10);
    WriteCmd((x & 0x0f) | 0x01);
}

void I2COLED_Fill(unsigned char fill_Data) //全屏填充
{
    unsigned char m, n;
    for (m = 0; m < 8; m++)
    {
        WriteCmd(0xb0 + m); // page0-page1
        WriteCmd(0x00);     // low column start address
        WriteCmd(0x10);     // high column start address
        for (n = 0; n < 128; n++)
        {
            WriteDat(fill_Data);
        }
    }
}

void I2COLED_CLS(void) //清屏幕
{
    I2COLED_Fill(0x00);
}

//--------------------------------------------------------------
// Prototype      : void I2COLED_ON(void)
// Calls          :
// Parameters     : none
// Description    : 将I2COLED 从休眠中唤醒
//--------------------------------------------------------------
void I2COLED_ON(void)
{
    WriteCmd(0X8D); // 设置电荷泵
    WriteCmd(0X14); // 开启电荷泵
    WriteCmd(0XAF); // I2COLED 唤醒
}

//--------------------------------------------------------------
// Prototype      : void I2COLED_OFF(void)
// Calls          :
// Parameters     : none
// Description    : 让OLE休眠 -- 休眠模式下，I2COLED功耗不到10uA
//--------------------------------------------------------------
void I2COLED_OFF(void)
{
    WriteCmd(0X8D); // 设置电荷泵
    WriteCmd(0X10); // 关闭电荷泵
    WriteCmd(0XAE); // I2COLED 休眠
}

//--------------------------------------------------------------
// Prototype      : void I2COLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch[],
// unsigned char TextSize) Calls          : Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); ch[]
// -- 要显示的字符串; TextSize -- 字符大小(1:6*8 ; 2:8*16) Description    :
// 显示codetab.h中的ASCII字符,有6*8和8*16可选择
//--------------------------------------------------------------
void I2COLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{

    unsigned char j = 0;

    switch (TextSize)
    {
    case 1: {
        while (ch[j] != '\0')
        {
            if (x > 122)
            {
                x = 0;
                y++;
            }

            I2COLED_ShowChar(x, y, ch[j++], TextSize);
            x += 6;
        }
    }
    break;
    case 2: {
        while (ch[j] != '\0')
        {
            if (x > 120)
            {
                x = 0;

                if (y != 0 && y % 2 == 0)
                {
                    y *= 2;
                }
                else
                {
                    y += 2;
                }
            }
            I2COLED_ShowChar(x, y, ch[j++], TextSize);
            x += 8;
        }
    }
    break;
    }
}

/**
 * @brief  I2COLED次方函数
 * @param  X        : 底数
 * @param  Y        : 指数
 * @retval 返回值等于X的Y次方
 */
uint32_t I2COLED_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--)
    {
        Result *= X;
    }
    return Result;
}

/**
 * @brief 显示codetab.h中的ASCII字符,有 6*8 和 8*16 可选择
 * @param x        : 横坐标的位置 x ∈ [0~127]
 * @param y        : 纵坐标的位置 y ∈ [0~7]
 * @param Num      : 要显示的无符号 32 位 整数
 * @param Length   : 要显示的数字长度
 * @param TextSize : 字符大小
 * @retval
 *
 */
void I2COLED_ShowNum(unsigned char x, unsigned char y, uint32_t Num, uint8_t Length,
                     unsigned char TextSize)
{
    unsigned char idx;
    switch (TextSize)
    {
    case 1: {
        if (x > 128 - Length * 6)
        {
            x = 0;
            y++;
        }

        //利用 单个数字+'0'显示字符， 即利用I2COLED_ShowChar来显示数字
        for (idx = 0; idx < Length; idx++)
        {
            // 6个像素作为一个字符宽度，所以下一个数字需要将位置向右移动6像素的位置
            I2COLED_ShowChar(x + idx * 6, y, Num / I2COLED_Pow(10, Length - idx - 1) % 10 + '0',
                             TextSize);
        }
    }
    break;
    case 2: {
        if (x > 128 - Length * 8) // 超过最右侧边缘就不能正常显示了
        {
            x = 0;
            if (y % 2 == 0)
            {
                y *= 2;
            }
            else
            {
                y += 2;
            }
        }

        for (idx = 0; idx < Length; idx++)
        {
            I2COLED_ShowChar(x + idx * 8, y, Num / I2COLED_Pow(10, Length - idx - 1) % 10 + '0',
                             TextSize);
        }
    }
    break;
    }
}

/**
 * @brief 显示codetab.h中的ASCII字符,有 6*8 和 8*16 可选择
 * @param x        : 横坐标的位置 x ∈ [0~127]
 * @param y        : 纵坐标的位置 y ∈ [0~7]
 * @param Num      : 要显示的无符号 32 位 整数
 * @param Length   : 要显示的数字长度
 * @param TextSize : 字符大小
 * @retval
 *
 */
void I2COLED_ShowSignedNum(unsigned char x, unsigned char y, int32_t Num, uint8_t Length,
                           unsigned char TextSize)
{
    unsigned char idx;
    uint32_t Number;

    switch (TextSize)
    {
    case 1: {
        // 此处减去1 是为了保证符号与数字在同一行中
        if (x > 128 - 1 - Length * 6)
        {
            x = 0;
            y++;
        }
        if (Num >= 0)
        {
            Number = Num;
            I2COLED_ShowChar(x, y, '+', TextSize);
        }
        else
        {
            Number = -Num;
            I2COLED_ShowChar(x, y, '-', TextSize);
        }

        x += 6;

        //利用 单个数字+'0'显示字符， 即利用I2COLED_ShowChar来显示数字
        for (idx = 0; idx < Length; idx++)
        {
            // 6个像素作为一个字符宽度，所以下一个数字需要将位置向右移动6像素的位置
            I2COLED_ShowChar(x + idx * 6, y, Number / I2COLED_Pow(10, Length - idx - 1) % 10 + '0',
                             TextSize);
        }
    }
    break;
    case 2: {
        if (x > 128 - 1 - Length * 8) // 超过120 最右侧边缘就不能正常显示了
        {
            x = 0;
            if (y % 2 == 0)
            {
                y *= 2;
            }
            else
            {
                y += 2;
            }
        }

        if (Num >= 0)
        {
            Number = Num;
            I2COLED_ShowChar(x, y, '+', TextSize);
        }
        else
        {
            Number = -Num;
            I2COLED_ShowChar(x, y, '-', TextSize);
        }
        x += 8;
        for (idx = 0; idx < Length; idx++)
        {
            I2COLED_ShowChar(x + idx * 8, y, Number / I2COLED_Pow(10, Length - idx - 1) % 10 + '0',
                             TextSize);
        }
    }
    break;
    }
}

/**
 * @brief 显示codetab.h中的ASCII字符,有 6*8 和 8*16 可选择
 * @param x        : 横坐标的位置 x ∈ [0~127]
 * @param y        : 纵坐标的位置 y ∈ [0~7]
 * @param Num      : 要显示的16进制数 0xAB
 * @param Length   : 要显示的字符长度
 * @param TextSize : 字符大小
 * @retval
 *
 */
void I2COLED_ShowHexNum(uint8_t x, uint8_t y, uint32_t Num, uint8_t Length, unsigned char TextSize)
{
    uint8_t i, SingleNum;
    switch (TextSize)
    {
    case 1: {

        if (x > 128 - Length * 6)
        {
            x = 0;
            y++;
        }
        if (y < 8)
        {
            for (i = 0; i < Length; i++)
            {
                SingleNum = Num / I2COLED_Pow(16, Length - i - 1) % 16;
                if (SingleNum < 10)
                {
                    I2COLED_ShowChar(x + i * 6, y, SingleNum + '0', TextSize);
                }
                else
                {
                    I2COLED_ShowChar(x + i * 6, y, SingleNum - 10 + 'A', TextSize);
                }
            }
        }
    }
    break;
    case 2: {

        if (x > 128 - Length * 8) // 超过预留字符长度(每个字符长8个像素) 最右侧边缘就不能正常显示了
        {
            x = 0;
            if (y % 2 == 0)
            {
                y *= 2;
            }
            else
            {
                y += 2;
            }
        }
        if (y < 8)
        {
            for (i = 0; i < Length; i++)
            {
                SingleNum = Num / I2COLED_Pow(16, Length - i - 1) % 16;
                if (SingleNum < 10)
                {
                    I2COLED_ShowChar(x + i * 8, y, SingleNum + '0', TextSize);
                }
                else
                {
                    I2COLED_ShowChar(x + i * 8, y, SingleNum - 10 + 'A', TextSize);
                }
            }
        }
    }
    break;
    }
}

/**
 * @brief 显示codetab.h中的ASCII字符,有 6*8 和 8*16 可选择
 * @param x        : 横坐标的位置 x ∈ [0~127]
 * @param y        : 纵坐标的位置 y ∈ [0~7]
 * @param Num      : 要显示的2进制数 0xAB
 * @param Length   : 要显示的字符长度
 * @param TextSize : 字符大小
 * @retval
 *
 */
void I2COLED_ShowBinNum(uint8_t x, uint8_t y, uint32_t Num, uint8_t Length, unsigned char TextSize)
{
    uint8_t i, SingleNum;
    switch (TextSize)
    {
    case 1: {

        if (x > 128 - Length * 6)
        {
            x = 0;
            y++;
        }

        if (y < 8)
        {
            for (i = 0; i < Length; i++)
            {
                SingleNum = Num / I2COLED_Pow(2, Length - i - 1) % 2;
                I2COLED_ShowChar(x + i * 6, y, SingleNum + '0', TextSize);
            }
        }
    }
    break;
    case 2: {

        if (x > 128 - Length * 8) // 超过预留字符长度(每个字符长8个像素) 最右侧边缘就不能正常显示了
        {
            x = 0;
            if (y % 2 == 0)
            {
                y *= 2;
            }
            else
            {
                y += 2;
            }
        }
        if (y < 8)
        {
            for (i = 0; i < Length; i++)
            {
                SingleNum = Num / I2COLED_Pow(2, Length - i - 1) % 2;
                I2COLED_ShowChar(x + i * 8, y, SingleNum + '0', TextSize);
            }
        }
    }
    break;
    }
}

/**
 * @brief 显示字符
 * @param x        : 横坐标的位置 x ∈ [0~127]
 * @param y        : 纵坐标的位置 y ∈ [0~7]
 * @param Char     : 要显示的字符
 * @param TextSize : 字符大小 1:字宽6个像素，2:字宽8个像素
 * @retval
 *
 */
void I2COLED_ShowChar(uint8_t x, uint8_t y, char Char, unsigned char TextSize)
{
    uint8_t i;
    Char = Char - 32;
    switch (TextSize)
    {
    case 1: {
        // 不足一个字符的宽度
        if (x > 122)
        {
            x = 0;
            y++;
        }
        I2COLED_SetPos(x, y);
        for (i = 0; i < 6; i++)
            WriteDat(F6x8[Char][i]);
        x += 6;
    }
    break;
    case 2: {
        if (x > 120)
        { // 不足一个字符的宽度			{
            x = 0;
            if (y % 2 == 0)
            {
                y *= 2;
            }
            else
            {
                y += 2;
            }
        }

        // 判断是否越界，越界就不再显示

        I2COLED_SetPos(x, y);
        for (i = 0; i < 8; i++)
            WriteDat(F8X16[Char * 16 + i]);
        I2COLED_SetPos(x, y + 1);

        for (i = 0; i < 8; i++)
            WriteDat(F8X16[Char * 16 + i + 8]);
    }
    break;
    }
}

//--------------------------------------------------------------
// Prototype      : void I2COLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
// Calls          :
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); N:汉字在codetab.h中的索引
// Description    : 显示codetab.h中的汉字,16*16点阵
//--------------------------------------------------------------
void I2COLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
    unsigned char wm   = 0;
    unsigned int adder = 32 * N;
    I2COLED_SetPos(x, y);
    for (wm = 0; wm < 16; wm++)
    {
        WriteDat(F16x16_[adder]);
        adder += 1;
    }
    I2COLED_SetPos(x, y + 1);
    for (wm = 0; wm < 16; wm++)
    {
        WriteDat(F16x16_[adder]);
        adder += 1;
    }
}

//--------------------------------------------------------------
// Prototype      : void I2COLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned
// char y1,unsigned char BMP[]); Calls          : Parameters     : x0,y0 -- 起始点坐标(x0:0~127,
// y0:0~7); x1,y1 -- 起点对角线(结束点)的坐标(x1:1~128,y1:1~8) Description    : 显示BMP位图
//--------------------------------------------------------------
void I2COLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1,
                     unsigned char BMP[])
{
    unsigned int j = 0;
    unsigned char x, y;

    if (y1 % 8 == 0)
        y = y1 / 8;
    else
        y = y1 / 8 + 1;
    for (y = y0; y < y1; y++)
    {
        I2COLED_SetPos(x0, y);
        for (x = x0; x < x1; x++)
        {
            WriteDat(BMP[j++]);
        }
    }
}
