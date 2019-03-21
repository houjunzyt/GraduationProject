#ifndef _ICONVIEWDEMO_H
#define _ICONVIEWDEMO_H
#include "DIALOG.h"


/* Structure for menu items */
typedef struct {
  char * sText;
  U16 Id;
  U16 Flags;
} MENU_ITEM;

extern GUI_CONST_STORAGE GUI_BITMAP bmbuttonuser1;
extern GUI_CONST_STORAGE GUI_BITMAP bmbuttonuser2;
void iconviewdemo(void); 
void k_StartUp(void);




#endif

