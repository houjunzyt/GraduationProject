#ifndef _BOARD_H_
#define _BOARD_H_

#include "stm32f4xx.h"
#include "usart.h"
#include "stm32f429i_discovery_ioe.h"
#include <stdlib.h>
#include "cpuusage.h"
#include "rtc.h"
#include <rthw.h>
#include <rtthread.h>
#include "rtconfig.h"

void rt_hw_board_init(void);
void SysTick_Handler(void);


#endif
