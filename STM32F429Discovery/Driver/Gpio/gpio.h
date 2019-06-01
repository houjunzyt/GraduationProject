#ifndef _GPIO_H_
#define _GPIO_H_

#include "stm32f4xx.h"
#include "bitband.h"

#define PORT1 PDout(4)
#define PORT2 PDout(5)
#define PORT3 PDout(7)
#define PORT4 PGout(9)

void GPIOInit(void);

#endif

