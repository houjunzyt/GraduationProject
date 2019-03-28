#ifndef _ICONVIEWDEMO_H
#define _ICONVIEWDEMO_H

#include "DIALOG.h"
#include "iconbitmap.h"
#include "background.h"
#include "DIALOG.h"
#include "FRAMEWIN.h"
#include "cpuusage.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "MESSAGEBOX.h"
#include "userguiid.h"
#include "getthreadinfo.h"
#include "kernellog.h"
#include "systemset.h"
#include "calender.h"

typedef struct {
  char * sText;
  U16 Id;
  U16 Flags;
} MENU_ITEM;

extern GUI_CONST_STORAGE GUI_BITMAP bmbuttonuser1;
extern GUI_CONST_STORAGE GUI_BITMAP bmbuttonuser2;
void iconviewdemo(void); 
void k_StartUp(void);
void k_UpdateLog(char *Msg);

#endif

