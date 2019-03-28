#ifndef _SYSTEMSET_H_
#define _SYSTEMSET_H_

#include "DIALOG.h"
#include "stm32f4xx.h"
#include <string.h>
#include "userguiid.h"
#include "rtc.h"

typedef struct
{
	uint8_t Button_Wifi;//wifiµÄ×´Ì¬°´Å¥
	
}SystemConfig;




extern GUI_CONST_STORAGE GUI_BITMAP bminfo;
void CreateSystemSetting(WM_HWIN hWin);

extern GUI_CONST_STORAGE GUI_BITMAP bmoff;
extern GUI_CONST_STORAGE GUI_BITMAP bmon;

void InitSystemConfigStruct(void);
SystemConfig * GetSystemConfigStruct(void);

#endif
