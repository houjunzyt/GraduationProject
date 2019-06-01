#ifndef _ESP8266_H_
#define _ESP8266_H_
#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "delay.h"
#include "rtc.h"
#include <string.h>
#include <stdlib.h>
#define RETRY_TIMES 5


uint8_t ESP8266_CloseEcho(void);
uint8_t ESP8266_SetWorkMode(int mode);
uint8_t ESP8266_ResetSys(void);
uint8_t ESP8266_SendCmd(char *cmd);
uint8_t ESP8266_Check_Exist(void);
uint8_t ESP8266_ConnectNum(uint8_t num);
uint8_t ESP8266_SetPortNum(uint8_t key,u32 port);
void ESP8266_Cmd(char *cmd);

/***************************************************
*******STA模式+TCP客户模式初始化相关函数声明********
***************************************************/
void ESP8266_STATCPClient_MODE_Init(void);
uint8_t ESP8266_STATCPClient_SsidPasswords(char *ssid,char *passwords);
uint8_t ESP8266_STAClient_Get_IP_MAC(void);
uint8_t ESP8266_STATCPClient_ConnectServer(char *server_ip,u32 port);
uint8_t ESP8266_STATCPClient_SetWorkMode(int mode); 
uint8_t ESP8266_STAClient_SendData(void);
void ESP8266_STAClient_ExitMode(void);

#endif

