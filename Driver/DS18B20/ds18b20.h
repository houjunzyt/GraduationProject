#ifndef __DS18B20_H
#define __DS18B20_H 

#include "stm32f4xx.h"
#include "bitband.h"
#include "board.h"

//IO方向设置
#define DS18B20_IO_IN()  {GPIOF->MODER&=~(3<<(6*2));GPIOF->MODER|=0<<(6*2);}	//PB14输入模式
#define DS18B20_IO_OUT() {GPIOF->MODER&=~(3<<(6*2));GPIOF->MODER|=1<<(6*2);} 	//PB14输出模式
 
////IO操作函数											   
#define	DS18B20_DQ_OUT PFout(6) //数据端口	 
#define	DS18B20_DQ_IN  PFin(6)  //数据端口	 
   	
void DS18B20_GPIO_Init(void);
uint8_t DS18B20_Init(void);			//初始化DS18B20
short DS18B20_Get_Temp(uint8_t index);	//获取温度
void DS18B20_Start(void);		//开始温度转换
void DS18B20_Write_Byte(uint8_t dat);//写入一个字节
uint8_t DS18B20_Read_Byte(void);		//读出一个字节
uint8_t DS18B20_Read_Bit(void);		//读出一个位
uint8_t DS18B20_Check(void);			//检测是否存在DS18B20
void DS18B20_Rst(void);			//复位DS18B20 
void GetRomId(uint8_t *IdRoom);

#endif
