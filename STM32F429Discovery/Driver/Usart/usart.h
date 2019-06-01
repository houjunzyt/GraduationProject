#ifndef __USART_H
#define __USART_H

#include "stdio.h"
#include "stm32f4xx.h"

/*----------------------USART配置宏 ------------------------*/

#define  USART1_BaudRate  115200

#define USART1_TX_PIN				GPIO_Pin_9					// TX 引脚
#define	USART1_TX_PORT			GPIOA							// TX 引脚端口
#define	USART1_TX_CLK				RCC_AHB1Periph_GPIOA		// TX 引脚时钟
#define USART1_TX_PinSource GPIO_PinSource9			// 引脚源

#define USART1_RX_PIN				GPIO_Pin_10             // RX 引脚
#define	USART1_RX_PORT			GPIOA                   // RX 引脚端口
#define	USART1_RX_CLK				RCC_AHB1Periph_GPIOA    // RX 引脚时钟
#define USART1_RX_PinSource GPIO_PinSource10        // 引脚源

#define USART5_BaudRate  115200

#define USART5_TX_PIN				GPIO_Pin_12					// TX 引脚
#define	USART5_TX_PORT			GPIOC							// TX 引脚端口
#define	USART5_TX_CLK				RCC_AHB1Periph_GPIOC		// TX 引脚时钟
#define USART5_TX_PinSource GPIO_PinSource12			// 引脚源

#define USART5_RX_PIN				GPIO_Pin_2             // RX 引脚
#define	USART5_RX_PORT			GPIOD                 // RX 引脚端口
#define	USART5_RX_CLK				RCC_AHB1Periph_GPIOD    // RX 引脚时钟
#define USART5_RX_PinSource GPIO_PinSource2        // 引脚源


/*---------------------- 函数声明 ----------------------------*/

void  Usart_Config (void);	// USART初始化函数
void  Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void  Usart_SendString( USART_TypeDef * pUSARTx, char *str);

#endif //__USART_H

