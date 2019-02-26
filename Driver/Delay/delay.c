#include "delay.h"

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
