#ifndef __BAGAPP_H
#define __BAGAPP_H
#include "stm32f4xx.h"
#include "WM.h"

extern WM_HWIN BagAPPWin;           //BagAPP窗口
extern WM_HWIN BookAPPWin;          //BookAPP窗口
extern WM_HWIN BookshAPPWin;        //BookshAPP窗口
extern WM_HWIN BrowerAPPWin;        //BrowerAPP窗口

void CreateBagAPP(void);            //创建BagAPP
void CreateBookAPP(void);           //创建BookAPP
void CreateBookshAPP(void);          //创建BookshAPP
void CreateBrowerAPP(void);         //创建BrowerAPP
#endif
