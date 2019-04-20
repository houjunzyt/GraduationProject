#ifndef __DS18B20_H
#define __DS18B20_H 

#include "stm32f4xx.h"
#include "bitband.h"
#include "board.h"

//IO��������
#define DS18B20_IO_IN()  {GPIOF->MODER&=~(3<<(6*2));GPIOF->MODER|=0<<(6*2);}	//PB14����ģʽ
#define DS18B20_IO_OUT() {GPIOF->MODER&=~(3<<(6*2));GPIOF->MODER|=1<<(6*2);} 	//PB14���ģʽ
 
////IO��������											   
#define	DS18B20_DQ_OUT PFout(6) //���ݶ˿�	 
#define	DS18B20_DQ_IN  PFin(6)  //���ݶ˿�	 
   	
void DS18B20_GPIO_Init(void);
uint8_t DS18B20_Init(void);			//��ʼ��DS18B20
short DS18B20_Get_Temp(uint8_t index);	//��ȡ�¶�
void DS18B20_Start(void);		//��ʼ�¶�ת��
void DS18B20_Write_Byte(uint8_t dat);//д��һ���ֽ�
uint8_t DS18B20_Read_Byte(void);		//����һ���ֽ�
uint8_t DS18B20_Read_Bit(void);		//����һ��λ
uint8_t DS18B20_Check(void);			//����Ƿ����DS18B20
void DS18B20_Rst(void);			//��λDS18B20 
void GetRomId(uint8_t *IdRoom);

#endif
