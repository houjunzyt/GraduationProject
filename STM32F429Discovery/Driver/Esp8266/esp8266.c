#include "esp8266.h"

extern uint8_t USART1_RX_BUFF[512];
extern uint32_t USART1_RX_CNT;
extern uint32_t USART1_RX_FLAG;

/**********************************************************
函数功能： STA模式+TCP客户端模式初始化
传入参数：
返回参数说明：
AT+CIPSEND\r\n    开始透传模式

**********************************************************/
void ESP8266_STATCPClient_MODE_Init()
{			 
		if(ESP8266_Check_Exist())
			printf("ESP8266 couldn't found!\n");	
		if(ESP8266_CloseEcho())
			printf("ESP8266 close echo fail!\n");	
		if(ESP8266_SetWorkMode(1))
			printf("ESP8266 set work mode fail!\n");
		if(ESP8266_ResetSys())
			printf("ESP8266 reset fail!\n");	
		if(ESP8266_CloseEcho())
			printf("ESP8266 close echo fail!\n");	
		DELAY_MS(200);
		if(ESP8266_STATCPClient_SsidPasswords("HJ","201530040302"))
			printf("ESP8266 connect ssid&passwords fail!\n");	
		DELAY_MS(1000);
		if(ESP8266_ConnectNum(0))
			printf("ESP8266 set connect setting fail!\n");
		if(ESP8266_STAClient_Get_IP_MAC())
			printf("ESP8266 get ip fail!\n");
		if(ESP8266_STATCPClient_ConnectServer("192.168.1.101",8000))
			printf("ESP8266 connect server fail!\n");
		if(ESP8266_STATCPClient_SetWorkMode(1))
			printf("ESP8266 set mode fail!\n");
		if(ESP8266_STAClient_SendData())
			printf("ESP8266 build connect fail!\n");
		Usart_SendString(USART1,"setID:1234");
}
/**********************************************************
函数功能：发送回执OK的命令
传入参数：命令
返回参数说明：1：执行失败，0：执行成功
***********************************************************/
uint8_t ESP8266_SendCmd(char *cmd)
{
	int i,j;
	USART1_RX_FLAG=0;
	USART1_RX_CNT=0;
	if(strcmp(cmd,"AT\r\n")==0)
	{
		ESP8266_STAClient_ExitMode();
		ESP8266_STATCPClient_SetWorkMode(0);
	}
	for(i=0;i<RETRY_TIMES;i++)
	{
		USART1_RX_FLAG=0;//标志位归位
		USART1_RX_CNT=0;//计数清零
		Usart_SendString(USART1,cmd);
		for(j=0;j<4;j++)
		{
			DELAY_MS(100);
			if(USART1_RX_FLAG)//收到一帧完整的数据
			{
				USART1_RX_BUFF[USART1_RX_CNT]='\0';//添加结束符
				printf("USART1 resive :%s \n",USART1_RX_BUFF);
				USART1_RX_FLAG=0;//标志位归位
				USART1_RX_CNT=0;//计数清零
				if(strstr((char *)USART1_RX_BUFF,"OK")!=NULL)
				{
					if(strstr(cmd,"CWMODE")!=NULL)
						printf("ESP8266 set work mode success!\n");
					else
						if(strstr(cmd,"CIPMUX")!=NULL)
							printf("ESP8266 connect setting set success！\n");
						else
							if(strstr(cmd,"E0")!=NULL)
								printf("ESP8266 close echo success！\n");
							else
								if(strstr(cmd,"CIPMODE")!=NULL)
									printf("ESP8266 STA CIPmode set success!\n");
								else
									printf("ESP8266 exist\n");
					return 0;
				}
				else		
					break;
			}
		}
	}
	return 1;
}
/**********************************************************
函数功能：检查设备是否存在
返回参数说明：1：执行失败，0：执行成功
***********************************************************/
uint8_t ESP8266_Check_Exist(void)
{
	return(ESP8266_SendCmd("AT\r\n"));
}
/**********************************************************
函数功能：设置模式
传入参数：1、2、3，对应的三种模式 1：station 2：ap 3:1&2
返回参数说明：1：执行失败，0：执行成功
**********************************************************/
uint8_t ESP8266_SetWorkMode(int mode) 
{
	char mode_cmd[20];
	switch(mode)
	{
		case 1 :strcpy(mode_cmd,"AT+CWMODE=1\r\n");break;
		case 2 :strcpy(mode_cmd,"AT+CWMODE=2\r\n");break;
		case 3 :strcpy(mode_cmd,"AT+CWMODE=3\r\n");break;
	}
	return(ESP8266_SendCmd(mode_cmd));
}
/**********************************************************
函数功能：AP模式+TCP服务器模式设置热点名称与连接热点密码
传入参数：
返回参数说明：1：执行失败，0：执行成功
说明:发送指令:AT+CWSAP="WBYQ_ESP8266","12345678",1,4
							OK
**********************************************************/
uint8_t ESP8266_APTCPServer_SetSsidPasswords(char *ssid,char *passwords)
{
	int i,j;
	char str[50];
	USART1_RX_FLAG=0;
	USART1_RX_CNT=0;
	sprintf(str,"AT+CWSAP=\"%s\",\"%s\",%d,%d\r\n",ssid,passwords,1,4);
	for(i=0;i<RETRY_TIMES;i++)
	{
		Usart_SendString(USART1,str);
		for(j=0;j<4;j++)
		{
			DELAY_MS(100);
			if(USART1_RX_FLAG)//收到一帧完整的数据
			{
				USART1_RX_BUFF[USART1_RX_CNT]='\0';//添加结束符
				printf("USART1 resive :%s \n",USART1_RX_BUFF);
				USART1_RX_FLAG=0;//标志位归位
				USART1_RX_CNT=0;//计数清零
				if(strstr((char *)USART1_RX_BUFF,"OK")!=NULL)
				{
					printf("ESP8266 set ssid:%s,PSW:%s success!\n",ssid,passwords);
					return 0;
				}
				else		
					break;
			}
		}
	}
	return 1;
}
/**********************************************************
函数功能：清除回显
返回参数说明：1：执行失败，0：执行成功
例子说明:发送指令:AT
									OK
***********************************************************/
uint8_t ESP8266_CloseEcho(void)
{
	return(ESP8266_SendCmd("ATE0\r\n"));
}
/**********************************************************
函数功能：设置可连接方式
传入参数：1：多连接模式 0:单连接模式
返回参数说明：1：执行失败，0：执行成功
例子说明：发送指令:AT+CIPMUX=1
									 OK
***********************************************************/
uint8_t ESP8266_ConnectNum(uint8_t num)
{
	char cmd[15];
	sprintf(cmd,"AT+CIPMUX=%d\r\n",num);
	return(ESP8266_SendCmd(cmd));
}
/**********************************************************
函数功能：复位
返回参数说明：1：执行失败，0：执行成功
由于复位会收到大波数据，暂时关闭串口使能，测试串口使能不关闭会导致崩溃
***********************************************************/	
uint8_t ESP8266_ResetSys(void)
{
	Usart_SendString(USART1,"AT+RST\r\n");
	printf("ESP8266 reset success!\n");
	USART1->CR1&=~(1<<13);//开启串口使能
	DELAY_MS(1000);
	DELAY_MS(1000);
	DELAY_MS(1000);
	DELAY_MS(1000);
	USART1->CR1|=1<<13;//开启串口使能
	return 0;
}

/**********************************************************
函数功能：STA模式+TCP客户端模式连接热点名称与连接热点密码
传入参数：
返回参数说明：1：执行失败，0：执行成功
AT+CWJAP="SSID","PassWord" 设置连接到的WIFI网络名称/加密方式/密码
                 连接成功会返回  WIFI GOT IP  字符串
**********************************************************/
uint8_t ESP8266_STATCPClient_SsidPasswords(char *ssid,char *passwords)
{
	int i,j;
	char str[50];
	USART1_RX_FLAG=0;
	USART1_RX_CNT=0;
	sprintf(str,"AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,passwords);
	for(i=0;i<RETRY_TIMES;i++)
	{
		Usart_SendString(USART1,str);
		for(j=0;j<4;j++)
		{
			DELAY_MS(4000);
			if(USART1_RX_FLAG)//收到一帧完整的数据
			{
				USART1_RX_BUFF[USART1_RX_CNT]='\0';//添加结束符
				USART1_RX_FLAG=0;//标志位归位
				USART1_RX_CNT=0;//计数清零 
				if(strstr((char *)USART1_RX_BUFF,"WIFI GOT IP")!=NULL)
				{
					printf("ESP8266 connect ssid:%s,PSW:%s success!\n",ssid,passwords);
					return 0;
				}
			}
		}
	}
	return 1;
}
/**********************************************************
函数功能：查询本地IP、MAC
传入参数：端口号
返回参数说明：1：执行失败，0：执行成功
发送指令:AT+CIFSR
				+CIFSR:STAIP,"192.168.43.177"
				+CIFSR:STAMAC,"84:f3:eb:17:ea:8b"
					OK
***********************************************************/
uint8_t ESP8266_STAClient_Get_IP_MAC(void)
{
	int i,j;
	char *temp1;
	char *temp2;
	USART1_RX_FLAG=0;
	USART1_RX_CNT=0;
	for(i=0;i<RETRY_TIMES;i++)
	{
		USART1_RX_FLAG=0;//标志位归位
		USART1_RX_CNT=0;//计数清零
		Usart_SendString(USART1,"AT+CIFSR\r\n");
		for(j=0;j<4;j++)
		{
			DELAY_MS(100);
			if(USART1_RX_FLAG)//收到一帧完整的数据
			{
				USART1_RX_BUFF[USART1_RX_CNT]='\0';//添加结束符
				printf("USART1 resive :%s \n",USART1_RX_BUFF);
				if(strstr((char *)USART1_RX_BUFF,"OK")!=NULL)
				{
					temp1=strstr((char *)USART1_RX_BUFF,"TAIP,");
					temp2=strstr(temp1+6,"\"");
					temp1=strstr((char *)USART1_RX_BUFF,"AMAC,");
					temp2=strstr(temp1+6,"\"");
					USART1_RX_FLAG=0;//标志位归位
					USART1_RX_CNT=0;//计数清零
					return 0;
				}
				else		
					break;
			}
		}
	}
	return 1;
}
/**********************************************************
函数功能：STA模式+TCP客户端模式连接服务器
传入参数：服务器IP，端口号
返回参数说明：1：执行失败，0：执行成功
AT+CIPSTART="TCP","119.29.140.191",8080
CONNECT
OK
**********************************************************/
uint8_t ESP8266_STATCPClient_ConnectServer(char *server_ip,u32 port)
{
	int i,j;
	char str[50];
	USART1_RX_FLAG=0;
	USART1_RX_CNT=0;
	sprintf(str,"AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",server_ip,port);
	for(i=0;i<RETRY_TIMES;i++)
	{
		Usart_SendString(USART1,str);
		for(j=0;j<4;j++)
		{
			DELAY_MS(1000);
			if(USART1_RX_FLAG)//收到一帧完整的数据
			{
				USART1_RX_BUFF[USART1_RX_CNT]='\0';//添加结束符
				printf("USART1 resive :%s \n",USART1_RX_BUFF);
				USART1_RX_FLAG=0;//标志位归位
				USART1_RX_CNT=0;//计数清零
				if(strstr((char *)USART1_RX_BUFF,"CONNECT")!=NULL)
				{
					printf("ESP8266 coonnect ip:%s,port:%d success!\n",server_ip,port);
					return 0;
				}
				else		
					break;
			}
		}
	}
	return 1;
}
/**********************************************************
函数功能：STA模式设置模式
传入参数：0、1对应的两种模式 0：普通模式 1：透传模式
返回参数说明：1：执行失败，0：执行成功
AT+CIPMODE=1
OK
**********************************************************/
uint8_t ESP8266_STATCPClient_SetWorkMode(int mode) 
{
	char mode_cmd[20];
	switch(mode)
	{
		case 0 :strcpy(mode_cmd,"AT+CIPMODE=0\r\n");break;
		case 1 :strcpy(mode_cmd,"AT+CIPMODE=1\r\n");break;
	}
	return(ESP8266_SendCmd(mode_cmd));
}
/**********************************************
函数功能：在STA模式下建立透明传输连接
传入参数：ID，数据
AT+CIPSEND
OK

> 
**********************************************/
uint8_t ESP8266_STAClient_SendData()
{
	int i,j;
	USART1_RX_FLAG=0;
	USART1_RX_CNT=0;
	for(i=0;i<RETRY_TIMES;i++)
	{
		USART1_RX_FLAG=0;//标志位归位
		USART1_RX_CNT=0;//计数清零
		Usart_SendString(USART1,"AT+CIPSEND\r\n");
		for(j=0;j<4;j++)
		{
			DELAY_MS(40);
			if(USART1_RX_FLAG)//收到一帧完整的数据
			{
				USART1_RX_BUFF[USART1_RX_CNT]='\0';//添加结束符
				printf("USART1 resive :%s \n",USART1_RX_BUFF);
				USART1_RX_FLAG=0;//标志位归位
				USART1_RX_CNT=0;//计数清零
				if(strstr((char *)USART1_RX_BUFF,">")!=NULL)//没有判断串口发送完毕
				{
					printf("ESP8266 build connect success!\n");
					return 0;
				}
				else		
					break;
			}
		}
	}
	return 1;
}
/******************************************************************
函数功能：STA模式退出透传模式
******************************************************************/
void ESP8266_STAClient_ExitMode()
{
	Usart_SendString(USART1,"+++\r\n");
	DELAY_MS(1000);
}
