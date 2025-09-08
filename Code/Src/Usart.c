/*=======================usart1.c======================*/
#include "Usart.h"

char Usart_RxPacket[100]; //定义接收数据包数组，数据包格式
uint8_t Usart_RxFlag;     //定义接收数据包标志位

/**
 * 函    数：串口初始化
 * 参    数：无
 * 返 回 值：无
 */
void Usart3_Init(void)
{
    /*开启时钟*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //开启USART1的时钟
    RCC_APB2PeriphClockCmd(USART3_GPIO_CLK, ENABLE);       //开启GPIOA的时钟

    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = USART3_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART3_GPIO_PORT, &GPIO_InitStructure); //将PA9引脚初始化为复用推挽输出

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin   = USART3_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART3_GPIO_PORT, &GPIO_InitStructure); //将PA10引脚初始化为上拉输入

    /*USART初始化*/
    USART_InitTypeDef USART_InitStructure;     //定义结构体变量
    USART_InitStructure.USART_BaudRate = 9600; //波特率
    USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;                             //硬件流控制，不需要
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; //模式，发送模式和接收模式均选择
    USART_InitStructure.USART_Parity     = USART_Parity_No;     //奇偶校验，不需要
    USART_InitStructure.USART_StopBits   = USART_StopBits_1;    //停止位，选择1位
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长，选择8位
    USART_Init(USART3, &USART_InitStructure); //将结构体变量交给USART_Init，配置USART1

    /*中断输出配置*/
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //开启串口接收数据的中断

    /*NVIC中断分组*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //配置NVIC为分组2

    /*NVIC配置*/
    NVIC_InitTypeDef NVIC_InitStructure;                      //定义结构体变量
    NVIC_InitStructure.NVIC_IRQChannel    = USART1_IRQn;      //选择配置NVIC的USART1线
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //指定NVIC线路使能
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //指定NVIC线路的抢占优先级为1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //指定NVIC线路的响应优先级为1
    NVIC_Init(&NVIC_InitStructure); //将结构体变量交给NVIC_Init，配置NVIC外设

    /*USART使能*/
    USART_Cmd(USART3, ENABLE); //使能USART1，串口开始运行
}

/**
 * 函    数：串口发送一个字节
 * 参    数：Byte 要发送的一个字节
 * 返 回 值：无
 */
void Usart3_SendByte(uint8_t Byte)
{
    USART_SendData(USART3, Byte); //将字节数据写入数据寄存器，写入后USART自动生成时序波形
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
        ; //等待发送完成
    /*下次写入数据寄存器会自动清除发送完成标志位，故此循环后，无需清除标志位*/
}

/**
 * 函    数：串口发送一个数组
 * 参    数：Array 要发送数组的首地址
 * 参    数：Length 要发送数组的长度
 * 返 回 值：无
 */
void Usart3_SendArray(uint8_t* Array, uint16_t Length)
{
    uint16_t i;
    for (i = 0; i < Length; i++) //遍历数组
    {
        Usart3_SendByte(Array[i]); //依次调用Usart_SendByte发送每个字节数据
    }
}

/**
 * 函    数：串口发送一个字符串
 * 参    数：String 要发送字符串的首地址
 * 返 回 值：无
 */
void Usart3_SendString(char* String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++) //遍历字符数组（字符串），遇到字符串结束标志位后停止
    {
        Usart3_SendByte(String[i]); //依次调用Usart_SendByte发送每个字节数据
    }
    // Usart_SendByte('\r');
    // Usart_SendByte('\n');
}

/**
 * 函    数：次方函数（内部使用）
 * 返 回 值：返回值等于X的Y次方
 */
uint32_t Usart3_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1; //设置结果初值为1
    while (Y--)          //执行Y次
    {
        Result *= X; //将X累乘到结果
    }
    return Result;
}

/**
 * 函    数：串口发送数字
 * 参    数：Number 要发送的数字，范围：0~4294967295
 * 参    数：Length 要发送数字的长度，范围：0~10
 * 返 回 值：无
 */
void Usart_SendNumber(uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++) //根据数字长度遍历数字的每一位
    {
        Usart3_SendByte(Number / Usart3_Pow(10, Length - i - 1) % 10 +
                        '0'); //依次调用Usart_SendByte发送每位数字
    }
}

/**
 * 函    数：使用printf需要重定向的底层函数(ARM_GCC)
 * 参    数：保持原始格式即可，无需变动
 * 返 回 值：保持原始格式即可，无需变动
 */

int _write(int fd, char* pBuffer, int size)
{
    for (int i = 0; i < size; i++)
    {
        while ((USART3->SR & 0x40) == 0)
            ; // wait last chuankou send ok
        USART3->DR = (uint8_t)pBuffer[i];
    }
    return size;
}

/**
 * 函    数：自己封装的prinf函数
 * 参    数：format 格式化字符串
 * 参    数：... 可变的参数列表
 * 返 回 值：无
 */
void Usart_Printf(char* format, ...)
{
    char String[100];              //定义字符数组
    va_list arg;                   //定义可变参数列表数据类型的变量arg
    va_start(arg, format);         //从format开始，接收参数列表到arg变量
    vsprintf(String, format, arg); //使用vsprintf打印格式化字符串和参数列表到字符数组中
    va_end(arg);                   //结束变量arg
    Usart3_SendString(String); //串口发送字符数组（字符串）
}

/**
 * 函    数：USART1中断函数
 * 参    数：无
 * 返 回 值：无
 * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
 *           函数名为预留的指定名称，可以从启动文件复制
 *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入
 */
// void USART1_IRQHandler(void)
// {
//     static uint8_t RxState   = 0; //定义表示当前状态机状态的静态变量
//     static uint8_t pRxPacket = 0; //定义表示当前接收数据位置的静态变量
//     if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) //判断是否是USART1的接收事件触发的中断
//     {
//         uint8_t RxData = USART_ReceiveData(USART1); //读取数据寄存器，存放在接收的数据变量

//         /*使用状态机的思路，依次处理数据包的不同部分*/

//         /*当前状态为1，接收数据包数据，同时判断是否接收到了第一个包尾*/
//         if (RxState == 0)
//         {
//             if (RxData == '\r') //如果收到第一个包尾
//             {
//                 RxState = 1; //置下一个状态
//             }
//             else //接收到了正常的数据
//             {
//                 Usart_RxPacket[pRxPacket] = RxData; //将数据存入数据包数组的指定位置
//                 pRxPacket++;                        //数据包的位置自增
//             }
//         }
//         /*当前状态为2，接收数据包第二个包尾*/
//         else if (RxState == 1)
//         {
//             if (RxData == '\n') //如果收到第二个包尾
//             {
//                 RxState                   = 0; //状态归0
//                 Usart_RxPacket[pRxPacket] = '\0'; //将收到的字符数据包添加一个字符串结束标志
//                 Usart_RxFlag = 1; //接收数据包标志位置1，成功接收一个数据包
//                 pRxPacket    = 0; //数据包的位置归零
//             }
//         }
//         USART_ClearITPendingBit(USART1, USART_IT_RXNE); //清除标志位
//     }
// }

// void Usart_SET(FunctionalState temp)
// {
//     USART_Cmd(USART1, temp);
// }