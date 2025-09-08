#include "OneWrite.h"
// #include "stdio.h" // printf用

// #define MaxSensorNum 8
// unsigned char
//     Onewrite_ID[MaxSensorNum]
//                [8]; // 存检测到的传感器Onewrite_ID的数组,前面的维数代表单根线传感器数量上限
// unsigned char Onewrite_SensorNum; // 检测到的传感器数量(从1开始，例如显示1代表1个，8代表8个)

// 配置Onewrite用到的I/O口
void Onewrite_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_Onewrite_GPIO_X, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = Onewrite_GPIO_NUM;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(Onewrite_GPIO_X, &GPIO_InitStructure);
    GPIO_SetBits(Onewrite_GPIO_X, Onewrite_GPIO_NUM);
}

// 引脚输入
void Onewrite_Mode_IPU(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin  = Onewrite_GPIO_NUM;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(Onewrite_GPIO_X, &GPIO_InitStructure);
}

// 引脚输出
void Onewrite_Mode_Out(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = Onewrite_GPIO_NUM;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(Onewrite_GPIO_X, &GPIO_InitStructure);
}

// 复位，主机给从机发送复位脉冲
void Onewrite_Rst(void)
{
    Onewrite_Mode_Out();
    Onewrite_DQ_OUT_Low; // 产生至少480us的低电平复位信号
    Delay_us(500);
    Onewrite_DQ_OUT_High; // 在产生复位信号后，需将总线拉高
    Delay_us(15);
}

// 检测从机给主机返回的应答脉冲。从机接收到主机的复位信号后，会在15~60us后给主机发一个应答脉冲
u8 Onewrite_Answer_Check(void)
{
    u8 delay = 0;
    Onewrite_Mode_IPU(); // 主机设置为上拉输入
    // 等待应答脉冲（一个60~240us的低电平信号 ）的到来
    // 如果100us内，没有应答脉冲，退出函数，注意：从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲
    while (Onewrite_DQ_IN && delay < 100)
    {
        delay++;
        Delay_us(1);
    }
    // 经过100us后，如果没有应答脉冲，退出函数
    if (delay >= 100) // Hu200
        return 1;
    else
        delay = 0;
    // 有应答脉冲，且存在时间不超过240us
    while (!Onewrite_DQ_IN && delay < 240)
    {
        delay++;
        Delay_us(1);
    }
    if (delay >= 240)
        return 1;
    return 0;
}

// 从Onewrite读取1个位
u8 Onewrite_Read_Bit(void)
{
    u8 data;
    Onewrite_Mode_Out();
    Onewrite_DQ_OUT_Low; // 读时间的起始：必须由主机产生 >1us <15us 的低电平信号
    Delay_us(2);
    Onewrite_DQ_OUT_High;
    Delay_us(12);
    Onewrite_Mode_IPU(); // 设置成输入，释放总线，由外部上拉电阻将总线拉高
    if (Onewrite_DQ_IN)
        data = 1;
    else
        data = 0;
    Delay_us(50);
    return data;
}

// 从Onewrite读取2个位
u8 Onewrite_Read_2Bit(void) //读二位 子程序
{
    u8 i;
    u8 dat = 0;
    for (i = 2; i > 0; i--)
    {
        dat = dat << 1;
        Onewrite_Mode_Out();
        Onewrite_DQ_OUT_Low;
        Delay_us(2);
        Onewrite_DQ_OUT_High;
        Onewrite_Mode_IPU();
        Delay_us(12);
        if (Onewrite_DQ_IN)
            dat |= 0x01;
        Delay_us(50);
    }
    return dat;
}

// 从Onewrite读取1个字节
u8 Onewrite_Read_Byte(void) // read one byte
{
    u8 i, j, dat;
    dat = 0;
    for (i = 0; i < 8; i++)
    {
        j   = Onewrite_Read_Bit();
        dat = (dat) | (j << i);
    }
    return dat;
}

// 写1位到Onewrite
void Onewrite_Write_Bit(u8 dat)
{
    Onewrite_Mode_Out();
    if (dat)
    {
        Onewrite_DQ_OUT_Low; // Write 1
        Delay_us(5);
        Onewrite_DQ_OUT_High;
        Delay_us(55);
    }
    else
    {
        Onewrite_DQ_OUT_Low; // Write 0
        Delay_us(55);
        Onewrite_DQ_OUT_High;
        Delay_us(5);
    }
}

// 写1字节到Onewrite
void Onewrite_Write_Byte(u8 dat)
{
    u8 j;
    u8 testb;
    Onewrite_Mode_Out();
    for (j = 1; j <= 8; j++)
    {
        testb = dat & 0x01;
        dat   = dat >> 1;
        if (testb)
        {
            Onewrite_DQ_OUT_Low; // 写1
            Delay_us(10);
            Onewrite_DQ_OUT_High;
            Delay_us(60);
        }
        else
        {
            Onewrite_DQ_OUT_Low; // 写0
            Delay_us(60);
            Onewrite_DQ_OUT_High; // 释放总线
            Delay_us(10);
        }
    }
}

//初始化Onewrite的IO口，同时检测DS的存在
u8 Onewrite_Init(void)
{
    Onewrite_GPIO_Config();
    Onewrite_Rst();
    return Onewrite_Answer_Check();
}

// 从Onewrite得到温度值，精度：0.1C，返回温度值（-550~1250），Temperature1返回浮点实际温度
float Onewrite_Get_Temp()
{
    // u8 flag;
    u8 j; //匹配的字节
    u16 tem, hum;
    uint8_t tpmsb, tplsb, Th, Tl, con, res, humil, humim, crc;
    float Temperature = 0;
    float Humidity    = 0;
    Onewrite_Rst();
    Onewrite_Answer_Check();
    Onewrite_Write_Byte(0xcc); // skip rom
    Onewrite_Write_Byte(0x44); // convert
    Onewrite_Rst();
    Onewrite_Answer_Check();

    Onewrite_Write_Byte(0xcc); // skip rom
    //匹配ID，i为形参
    // Onewrite_Write_Byte(0x55);
    // for (j = 0; j < 8; j++)
    // {
    //     Onewrite_Write_Byte(Onewrite_ID[i][j]);
    // }

    Onewrite_Write_Byte(0xbe); // convert

    tplsb = Onewrite_Read_Byte();
    tpmsb = Onewrite_Read_Byte();
    Th    = Onewrite_Read_Byte();
    Tl    = Onewrite_Read_Byte();
    con   = Onewrite_Read_Byte();
    res   = Onewrite_Read_Byte();
    humil = Onewrite_Read_Byte();
    humim = Onewrite_Read_Byte();
    crc   = Onewrite_Read_Byte();
    tpmsb = 0x0f & tpmsb;

    tem = ((tpmsb << 8) | tplsb); //温度拼接
    hum = ((humil << 8) | humim); //湿度拼接

    Temperature = (175.0 * (float)tem / 4095.0 - 45.0); // T = -45 + 175 * tem / (2^16-1)
    Humidity    = (100.0 * (float)hum / 65535.0);       // RH = hum*100 / (2^16-1)

    return Temperature;
}

// 自动搜索ROM
// void Onewrite_Search_Rom(void)
// {
//     u8 k, l, chongtuwei, m, n, num;
//     u8 zhan[5];
//     u8 ss[64];
//     u8 tempp;
//     l   = 0;
//     num = 0;
//     do
//     {
//         Onewrite_Rst(); //注意：复位的延时不够
//         Delay_us(480);  // 480、720
//         Onewrite_Write_Byte(0xf0);
//         for (m = 0; m < 8; m++)
//         {
//             u8 s = 0;
//             for (n = 0; n < 8; n++)
//             {
//                 k = Onewrite_Read_2Bit(); //读两位数据

//                 k = k & 0x03;
//                 s >>= 1;
//                 if (k == 0x01) // 01读到的数据为0 写0 此位为0的器件响应
//                 {
//                     Onewrite_Write_Bit(0);
//                     ss[(m * 8 + n)] = 0;
//                 }
//                 else if (k == 0x02) //读到的数据为1 写1 此位为1的器件响应
//                 {
//                     s = s | 0x80;
//                     Onewrite_Write_Bit(1);
//                     ss[(m * 8 + n)] = 1;
//                 }
//                 else if (k == 0x00) //读到的数据为00 有冲突位 判断冲突位
//                 {
//                     //如果冲突位大于栈顶写0 小于栈顶写以前数据 等于栈顶写1
//                     chongtuwei = m * 8 + n + 1;
//                     if (chongtuwei > zhan[l])
//                     {
//                         Onewrite_Write_Bit(0);
//                         ss[(m * 8 + n)] = 0;
//                         zhan[++l]       = chongtuwei;
//                     }
//                     else if (chongtuwei < zhan[l])
//                     {
//                         s = s | ((ss[(m * 8 + n)] & 0x01) << 7);
//                         Onewrite_Write_Bit(ss[(m * 8 + n)]);
//                     }
//                     else if (chongtuwei == zhan[l])
//                     {
//                         s = s | 0x80;
//                         Onewrite_Write_Bit(1);
//                         ss[(m * 8 + n)] = 1;
//                         l               = l - 1;
//                     }
//                 }
//                 else
//                 {
//                     //没有搜索到
//                 }
//             }
//             tempp               = s;
//             Onewrite_ID[num][m] = tempp; // 保存搜索到的ID
//         }
//         num = num + 1; // 保存搜索到的个数
//     } while (zhan[l] != 0 && (num < MaxSensorNum));
//     Onewrite_SensorNum = num;
//     // printf("Onewrite_SensorNum=%d\r\n",Onewrite_SensorNum);
// }