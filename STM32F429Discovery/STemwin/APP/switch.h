#ifndef _SWITCH_H_
#define _SWITCH_H_

#include "DIALOG.h"
#include "stm32f4xx.h"
#include <string.h>
#include "userguiid.h"

typedef struct
{
	uint8_t Air_conditioner;
	uint8_t Water_heater;
	uint8_t Washing;
	uint8_t	Plug;
}Switchconfig;

extern GUI_CONST_STORAGE GUI_BITMAP bmoff;
extern GUI_CONST_STORAGE GUI_BITMAP bmon;

WM_HWIN CreateSwitch(void);

#endif


