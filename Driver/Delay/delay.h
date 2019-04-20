#ifndef _DELAY_H_
#define _DELAY_H_ 
#include "stm32f4xx.h"

void Delay_ms(__IO u32 nTime);
uint32_t DWT_Delay_Init(void);

__STATIC_INLINE void DWT_Delay_us(volatile uint32_t microseconds)
{
  uint32_t clk_cycle_start = DWT->CYCCNT;
  /* Go to number of cycles for system */
  microseconds *= (SystemCoreClock / 1000000);
  /* Delay till end */
  while ((DWT->CYCCNT - clk_cycle_start) < microseconds);
}

#endif
