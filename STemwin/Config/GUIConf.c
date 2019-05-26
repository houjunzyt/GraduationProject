#include "GUI.h"
#include "stm32f4xx.h"
#include <rtthread.h>
#include <stdlib.h>

#define GUI_NUMBYTES  1024 * 4096	// 分配4M字节的SDRAM给emWin

uint32_t aMemory[GUI_NUMBYTES/4] __EXRAM;
void GUI_X_Config(void) 
{
  GUI_ALLOC_AssignMemory(aMemory, GUI_NUMBYTES);
}

