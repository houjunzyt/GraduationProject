#include "GUI.h"
#include "stm32f4xx.h"
#include <rtthread.h>
#include <stdlib.h>

#define GUI_NUMBYTES  1024 * 2096	// 分配4M字节的SDRAM给emWin

void GUI_X_Config(void) 
{
	uint32_t * aMemory=NULL;
	aMemory=(uint32_t *)malloc(GUI_NUMBYTES);
	if(aMemory==NULL)
	{
		rt_kprintf("aMemory malloc fail\n");
	}
  GUI_ALLOC_AssignMemory(aMemory, GUI_NUMBYTES);
}

