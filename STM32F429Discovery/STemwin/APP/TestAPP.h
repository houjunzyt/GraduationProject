#ifndef __BAGAPP_H
#define __BAGAPP_H
#include "stm32f4xx.h"
#include "WM.h"

extern WM_HWIN BagAPPWin;           //BagAPP����
extern WM_HWIN BookAPPWin;          //BookAPP����
extern WM_HWIN BookshAPPWin;        //BookshAPP����
extern WM_HWIN BrowerAPPWin;        //BrowerAPP����

void CreateBagAPP(void);            //����BagAPP
void CreateBookAPP(void);           //����BookAPP
void CreateBookshAPP(void);          //����BookshAPP
void CreateBrowerAPP(void);         //����BrowerAPP
#endif
