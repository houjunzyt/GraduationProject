#include "delay.h"
#include "rtthread.h"

u32 TimingDelay=0;//用于计量Systick中断次数

///***************************************************
//*函数功能：10微秒单位延时
//*传入参数：nTime：延时时间：nTime*10us
//*返回值  ：无
//***************************************************/
//void Delay_us(__IO u32 nTime)
//{
//	TimingDelay=nTime;
//	while (TimingDelay != 0);
//}

/***************************************************
*函数功能：毫秒单位延时
*传入参数：nTime：延时时间：nTime*1ms
*返回值  ：无
***************************************************/
void Delay_ms(__IO u32 Time)
{
	TimingDelay=Time;
	while (TimingDelay != 0);
}


uint32_t DWT_Delay_Init(void) 
{
  /* Disable TRC */
  CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // ~0x01000000;
  /* Enable TRC */
  CoreDebug->DEMCR |=  CoreDebug_DEMCR_TRCENA_Msk; // 0x01000000;

  /* Disable clock cycle counter */
  DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //~0x00000001;
  /* Enable  clock cycle counter */
  DWT->CTRL |=  DWT_CTRL_CYCCNTENA_Msk; //0x00000001;

  /* Reset the clock cycle counter value */
  DWT->CYCCNT = 0;

     /* 3 NO OPERATION instructions */
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");

  /* Check if clock cycle counter has started */
	if(DWT->CYCCNT)
	{
		return 0; /*clock cycle counter started*/
	}
	else
	{
		return 1; /*clock cycle counter not started*/
	}
}

void DELAY_MS(uint32_t ms)
{	
	rt_thread_delay(ms);
}
