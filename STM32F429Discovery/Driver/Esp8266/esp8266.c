#include "esp8266.h"

extern uint8_t USART1_RX_BUFF[512];
extern uint32_t USART1_RX_CNT;
extern uint32_t USART1_RX_FLAG;

/**********************************************************
�������ܣ� STAģʽ+TCP�ͻ���ģʽ��ʼ��
���������
���ز���˵����
AT+CIPSEND\r\n    ��ʼ͸��ģʽ

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
�������ܣ����ͻ�ִOK������
�������������
���ز���˵����1��ִ��ʧ�ܣ�0��ִ�гɹ�
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
		USART1_RX_FLAG=0;//��־λ��λ
		USART1_RX_CNT=0;//��������
		Usart_SendString(USART1,cmd);
		for(j=0;j<4;j++)
		{
			DELAY_MS(100);
			if(USART1_RX_FLAG)//�յ�һ֡����������
			{
				USART1_RX_BUFF[USART1_RX_CNT]='\0';//��ӽ�����
				printf("USART1 resive :%s \n",USART1_RX_BUFF);
				USART1_RX_FLAG=0;//��־λ��λ
				USART1_RX_CNT=0;//��������
				if(strstr((char *)USART1_RX_BUFF,"OK")!=NULL)
				{
					if(strstr(cmd,"CWMODE")!=NULL)
						printf("ESP8266 set work mode success!\n");
					else
						if(strstr(cmd,"CIPMUX")!=NULL)
							printf("ESP8266 connect setting set success��\n");
						else
							if(strstr(cmd,"E0")!=NULL)
								printf("ESP8266 close echo success��\n");
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
�������ܣ�����豸�Ƿ����
���ز���˵����1��ִ��ʧ�ܣ�0��ִ�гɹ�
***********************************************************/
uint8_t ESP8266_Check_Exist(void)
{
	return(ESP8266_SendCmd("AT\r\n"));
}
/**********************************************************
�������ܣ�����ģʽ
���������1��2��3����Ӧ������ģʽ 1��station 2��ap 3:1&2
���ز���˵����1��ִ��ʧ�ܣ�0��ִ�гɹ�
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
�������ܣ�APģʽ+TCP������ģʽ�����ȵ������������ȵ�����
���������
���ز���˵����1��ִ��ʧ�ܣ�0��ִ�гɹ�
˵��:����ָ��:AT+CWSAP="WBYQ_ESP8266","12345678",1,4
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
			if(USART1_RX_FLAG)//�յ�һ֡����������
			{
				USART1_RX_BUFF[USART1_RX_CNT]='\0';//��ӽ�����
				printf("USART1 resive :%s \n",USART1_RX_BUFF);
				USART1_RX_FLAG=0;//��־λ��λ
				USART1_RX_CNT=0;//��������
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
�������ܣ��������
���ز���˵����1��ִ��ʧ�ܣ�0��ִ�гɹ�
����˵��:����ָ��:AT
									OK
***********************************************************/
uint8_t ESP8266_CloseEcho(void)
{
	return(ESP8266_SendCmd("ATE0\r\n"));
}
/**********************************************************
�������ܣ����ÿ����ӷ�ʽ
���������1��������ģʽ 0:������ģʽ
���ز���˵����1��ִ��ʧ�ܣ�0��ִ�гɹ�
����˵��������ָ��:AT+CIPMUX=1
									 OK
***********************************************************/
uint8_t ESP8266_ConnectNum(uint8_t num)
{
	char cmd[15];
	sprintf(cmd,"AT+CIPMUX=%d\r\n",num);
	return(ESP8266_SendCmd(cmd));
}
/**********************************************************
�������ܣ���λ
���ز���˵����1��ִ��ʧ�ܣ�0��ִ�гɹ�
���ڸ�λ���յ������ݣ���ʱ�رմ���ʹ�ܣ����Դ���ʹ�ܲ��رջᵼ�±���
***********************************************************/	
uint8_t ESP8266_ResetSys(void)
{
	Usart_SendString(USART1,"AT+RST\r\n");
	printf("ESP8266 reset success!\n");
	USART1->CR1&=~(1<<13);//��������ʹ��
	DELAY_MS(1000);
	DELAY_MS(1000);
	DELAY_MS(1000);
	DELAY_MS(1000);
	USART1->CR1|=1<<13;//��������ʹ��
	return 0;
}

/**********************************************************
�������ܣ�STAģʽ+TCP�ͻ���ģʽ�����ȵ������������ȵ�����
���������
���ز���˵����1��ִ��ʧ�ܣ�0��ִ�гɹ�
AT+CWJAP="SSID","PassWord" �������ӵ���WIFI��������/���ܷ�ʽ/����
                 ���ӳɹ��᷵��  WIFI GOT IP  �ַ���
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
			if(USART1_RX_FLAG)//�յ�һ֡����������
			{
				USART1_RX_BUFF[USART1_RX_CNT]='\0';//��ӽ�����
				USART1_RX_FLAG=0;//��־λ��λ
				USART1_RX_CNT=0;//�������� 
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
�������ܣ���ѯ����IP��MAC
����������˿ں�
���ز���˵����1��ִ��ʧ�ܣ�0��ִ�гɹ�
����ָ��:AT+CIFSR
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
		USART1_RX_FLAG=0;//��־λ��λ
		USART1_RX_CNT=0;//��������
		Usart_SendString(USART1,"AT+CIFSR\r\n");
		for(j=0;j<4;j++)
		{
			DELAY_MS(100);
			if(USART1_RX_FLAG)//�յ�һ֡����������
			{
				USART1_RX_BUFF[USART1_RX_CNT]='\0';//��ӽ�����
				printf("USART1 resive :%s \n",USART1_RX_BUFF);
				if(strstr((char *)USART1_RX_BUFF,"OK")!=NULL)
				{
					temp1=strstr((char *)USART1_RX_BUFF,"TAIP,");
					temp2=strstr(temp1+6,"\"");
					temp1=strstr((char *)USART1_RX_BUFF,"AMAC,");
					temp2=strstr(temp1+6,"\"");
					USART1_RX_FLAG=0;//��־λ��λ
					USART1_RX_CNT=0;//��������
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
�������ܣ�STAģʽ+TCP�ͻ���ģʽ���ӷ�����
���������������IP���˿ں�
���ز���˵����1��ִ��ʧ�ܣ�0��ִ�гɹ�
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
			if(USART1_RX_FLAG)//�յ�һ֡����������
			{
				USART1_RX_BUFF[USART1_RX_CNT]='\0';//��ӽ�����
				printf("USART1 resive :%s \n",USART1_RX_BUFF);
				USART1_RX_FLAG=0;//��־λ��λ
				USART1_RX_CNT=0;//��������
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
�������ܣ�STAģʽ����ģʽ
���������0��1��Ӧ������ģʽ 0����ͨģʽ 1��͸��ģʽ
���ز���˵����1��ִ��ʧ�ܣ�0��ִ�гɹ�
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
�������ܣ���STAģʽ�½���͸����������
���������ID������
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
		USART1_RX_FLAG=0;//��־λ��λ
		USART1_RX_CNT=0;//��������
		Usart_SendString(USART1,"AT+CIPSEND\r\n");
		for(j=0;j<4;j++)
		{
			DELAY_MS(40);
			if(USART1_RX_FLAG)//�յ�һ֡����������
			{
				USART1_RX_BUFF[USART1_RX_CNT]='\0';//��ӽ�����
				printf("USART1 resive :%s \n",USART1_RX_BUFF);
				USART1_RX_FLAG=0;//��־λ��λ
				USART1_RX_CNT=0;//��������
				if(strstr((char *)USART1_RX_BUFF,">")!=NULL)//û���жϴ��ڷ������
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
�������ܣ�STAģʽ�˳�͸��ģʽ
******************************************************************/
void ESP8266_STAClient_ExitMode()
{
	Usart_SendString(USART1,"+++\r\n");
	DELAY_MS(1000);
}
