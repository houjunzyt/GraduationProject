#ifndef __USART_H
#define __USART_H

#include "stdio.h"
#include "stm32f4xx.h"

/*----------------------USART���ú� ------------------------*/

#define  USART1_BaudRate  115200

#define USART1_TX_PIN				GPIO_Pin_9					// TX ����
#define	USART1_TX_PORT			GPIOA							// TX ���Ŷ˿�
#define	USART1_TX_CLK				RCC_AHB1Periph_GPIOA		// TX ����ʱ��
#define USART1_TX_PinSource GPIO_PinSource9			// ����Դ

#define USART1_RX_PIN				GPIO_Pin_10             // RX ����
#define	USART1_RX_PORT			GPIOA                   // RX ���Ŷ˿�
#define	USART1_RX_CLK				RCC_AHB1Periph_GPIOA    // RX ����ʱ��
#define USART1_RX_PinSource GPIO_PinSource10        // ����Դ

#define USART5_BaudRate  115200

#define USART5_TX_PIN				GPIO_Pin_12					// TX ����
#define	USART5_TX_PORT			GPIOC							// TX ���Ŷ˿�
#define	USART5_TX_CLK				RCC_AHB1Periph_GPIOC		// TX ����ʱ��
#define USART5_TX_PinSource GPIO_PinSource12			// ����Դ

#define USART5_RX_PIN				GPIO_Pin_2             // RX ����
#define	USART5_RX_PORT			GPIOD                 // RX ���Ŷ˿�
#define	USART5_RX_CLK				RCC_AHB1Periph_GPIOD    // RX ����ʱ��
#define USART5_RX_PinSource GPIO_PinSource2        // ����Դ


/*---------------------- �������� ----------------------------*/

void  Usart_Config (void);	// USART��ʼ������
void  Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void  Usart_SendString( USART_TypeDef * pUSARTx, char *str);

#endif //__USART_H

