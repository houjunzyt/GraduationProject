#include "usart.h"  
#include "timer.h"
 uint8_t USART1_RX_BUFF[512];
 uint32_t USART1_RX_CNT;
 uint32_t USART1_RX_FLAG;

// 函数：usart IO口初始化
//
void  USART_GPIO_Config	(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd ( USART1_TX_CLK|USART1_RX_CLK, ENABLE); 	//IO口时钟配置
	RCC_AHB1PeriphClockCmd ( USART5_TX_CLK|USART5_RX_CLK, ENABLE); 	//IO口时钟配置
	//IO配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;   	 //复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //推挽
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		 //上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //速度等级

	//初始化 TX	引脚
	GPIO_InitStructure.GPIO_Pin = USART1_TX_PIN;	 
	GPIO_Init(USART1_TX_PORT, &GPIO_InitStructure);	
	//初始化 RX 引脚													   
	GPIO_InitStructure.GPIO_Pin = USART1_RX_PIN;	
	GPIO_Init(USART1_RX_PORT, &GPIO_InitStructure);		
	
	
	//初始化 TX	引脚
	GPIO_InitStructure.GPIO_Pin = USART5_TX_PIN;	 
	GPIO_Init(USART5_TX_PORT, &GPIO_InitStructure);	
	//初始化 RX 引脚													   
	GPIO_InitStructure.GPIO_Pin = USART5_RX_PIN;	
	GPIO_Init(USART5_RX_PORT, &GPIO_InitStructure);	
	
	
	//IO复用，复用到USART1
	GPIO_PinAFConfig(USART1_TX_PORT,USART1_TX_PinSource,GPIO_AF_USART1); 
	GPIO_PinAFConfig(USART1_RX_PORT,USART1_RX_PinSource,GPIO_AF_USART1);	
	
	GPIO_PinAFConfig(USART5_TX_PORT,USART5_TX_PinSource,GPIO_AF_UART5); 
	GPIO_PinAFConfig(USART5_RX_PORT,USART5_RX_PinSource,GPIO_AF_UART5);	
	
}

// 函数：串口中断优先级配置
//
void USART_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置中断分组2
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; // 选择 USART1 通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级设置为3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //响应优先级设置为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //开启中断
	NVIC_Init(&NVIC_InitStructure); //初始化配置
}

// 函数：USART 口初始化
//
void Usart_Config(void)
{		
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //开启串口时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE); //开启串口时钟	
	USART_GPIO_Config();	// IO口初始化
	USART_NVIC_Config(); //	串口中断初始化 

	USART_InitStructure.USART_BaudRate 	 = USART1_BaudRate; //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位8位
	USART_InitStructure.USART_StopBits   = USART_StopBits_1; //停止位1位
	USART_InitStructure.USART_Parity     = USART_Parity_No ; //无校验
	USART_InitStructure.USART_Mode 	    = USART_Mode_Rx | USART_Mode_Tx; //发送和接收模式
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 不使用硬件流控制
	
	USART_Init(USART1,&USART_InitStructure); //初始化串口1	
	USART_Init(UART5,&USART_InitStructure); //初始化串口1	
	USART_Cmd(USART1,ENABLE);	//使能串口1
	USART_Cmd(UART5,ENABLE);	//使能串口1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//使能串口中断接收
	
}

// 函数：重定义fputc函数
//
int fputc(int c, FILE *fp)
{
	USART_SendData( UART5,(u8)c );	// 发送单字节数据
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	//等待发送完毕 

	return (c); //返回字符
}
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

void	USART1_IRQHandler (void)
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)	
	{		
		TIM_Cmd(BASIC_TIM, ENABLE);	
		TIM6->CNT=0;//清空计数值		
		USART1_RX_BUFF[USART1_RX_CNT] = USART_ReceiveData( USART1 );	//将接收到的串口数据存入数组
		USART1_RX_CNT++;
	}	 	
}
