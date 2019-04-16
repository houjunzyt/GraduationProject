#ifndef _TEMPERATURE_H_
#define _TEMPERATURE_H_

#include "DIALOG.h"
#include "FRAMEWIN.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "MESSAGEBOX.h"
#include "userguiid.h"
#include "stm32f4xx.h"
#include "DIALOG.h"

#define ID_IMAGE_0_IMAGE_0   0x00
#define ID_IMAGE_1_IMAGE_0   0x01

void CreateTempAPP(WM_HWIN hWin);
WM_HWIN CreateTempNum(void);


#endif
