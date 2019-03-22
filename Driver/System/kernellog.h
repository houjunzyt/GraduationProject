#ifndef _KERNELLOG_H_
#define _KERNELLOG_H_

#include "iconviewdemo.h"
#include "board.h"
#include "rtthread.h"
#include "stm32f4xx.h"
#include <rthw.h>
#include <string.h>
#include <stdio.h>

extern uint8_t  *pLOG_CacheBuffer;  
extern uint32_t LOG_IN_ptr;

void KernelLogInit(void);

#endif
