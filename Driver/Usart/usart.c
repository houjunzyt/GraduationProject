#include "usart.h"

/**********************************************************
*��������:����1GPIO��ʼ��
*�������:��
*����ֵ  :��
**********************************************************/
void USART1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd ( USART1_TX_CLK|USART1_RX_CLK, ENABLE); 	//IO��ʱ������

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;   	 //����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //����
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;		 //����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //�ٶȵȼ�

	GPIO_InitStructure.GPIO_Pin = USART1_TX_PIN;		//��ʼ�� TX	���� 
	GPIO_Init(USART1_TX_PORT, &GPIO_InitStructure);	
												   
	GPIO_InitStructure.GPIO_Pin = USART1_RX_PIN;		//��ʼ�� RX ����	
	GPIO_Init(USART1_RX_PORT, &GPIO_InitStructure);		
	
	GPIO_PinAFConfig(USART1_TX_PORT,USART1_TX_PinSource,GPIO_AF_USART1); 	//IO���ã����õ�USART1
	GPIO_PinAFConfig(USART1_RX_PORT,USART1_RX_PinSource,GPIO_AF_USART1);	
}

/**********************************************************
*��������:����1��ʼ��
*�������:��
*����ֵ  :��
**********************************************************/
void Usart1_Config(void)
{		
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//��ʱ��
	USART1_GPIO_Config();	// IO�ڳ�ʼ�� 
	// ���ô��ڸ������
	USART_InitStructure.USART_BaudRate 	 = USART1_BaudRate; //������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //����λ8λ
	USART_InitStructure.USART_StopBits   = USART_StopBits_1; //ֹͣλ1λ
	USART_InitStructure.USART_Parity     = USART_Parity_No ; //��У��
	USART_InitStructure.USART_Mode 	    = USART_Mode_Rx | USART_Mode_Tx; //���ͺͽ���ģʽ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��ʹ��Ӳ��������
	
	USART_Init(USART1,&USART_InitStructure); //��ʼ������1
	USART_Cmd(USART1,ENABLE);	//ʹ�ܴ���1
}

///**********************************************************
//*��������:����Ӷ���
//*�������:��
//*����ֵ  :��
//**********************************************************/
//int fputc(int c, FILE *fp)
//{
//	USART_SendData( USART1,(u8)c );	// ���͵��ֽ�����
//	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	//�ȴ�������� 
//	return (c); //�����ַ�
//}
