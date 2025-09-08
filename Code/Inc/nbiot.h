#ifndef __NBIOT_H
#define __NBIOT_H

#include "Delay.h"
#include "NBIOT_Usart.h"
#include "stdbool.h"
#include "stm32f10x.h"
#include "string.h"
#include <stdint.h>

// extern char CIMI_Packet[100]; //定义接收数据包数组CIMI

#define nbiot_restart_clk RCC_APB2Periph_GPIOA
#define nbiot_restart_port GPIOA
#define nbiot_restart_pin GPIO_Pin_11

/*-----------------------ATbase-----------------------*/

#define AT_NB_at "AT"            // AT测试指令（OK）
#define AT_NB_clk "AT+QSCLK=0"   //禁⽤休眠模式
#define AT_NB_reboot "AT+QRST=1" //重启模组
#define AT_NB_cereg "AT+CEREG?"  //获取注⽹状态，0,1表⽰注⽹成功
#define AT_NB_ip "AT+CGPADDR?"   //获取IP
#define AT_NB_cimi "AT+CIMI"     //获取CIMI
#define AT_NB_csq "AT+CSQ"       //获取信号质量
#define AT_NB_ate_off "ATE0"     //设置回显关
#define AT_NB_ate_on "ATE1"      //设置回显关
#define AT_NB_end "\r\n"

/*-----------------------ATtcp-----------------------*/
// clang-format off
#define AT_NB_tcp_data_formate "AT+QICFG=\"dataformat\",0,0" //设置收发数据格式
#define AT_NB_tcp_connect "AT+QIOPEN=0,0,\"TCP\",\"218.93.208.52\",50011"
#define AT_NB_tcp_send_head "AT+QISEND="
#define AT_NB_tcp_close "AT+QICLOSE=0"

//clang-format on


extern uint8_t ERROR_FLAG;
// bool NBIOT_send_command(char* command, char* ans,uint8_t num);
// void nbiot_Init();
// void nbiot_get_cimi();
// void nbiot_connect(void);
// void nbiot_send_data(char* data);

u8 NB_IotConect();
u8 NB_IoT_conect_send(char* data);\
void nbiot_init();

// void nbiot_Init(void);
// void nbiot_connect(void);
#endif