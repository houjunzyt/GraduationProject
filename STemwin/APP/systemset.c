#include "systemset.h"

SystemConfig syssetting={0};

static const GUI_WIDGET_CREATE_INFO _aDialog[] = 
{
  { FRAMEWIN_CreateIndirect, "System", ID_FRAMEWIN_INFO, 0, 0, 240, 294, 0, 0x64, 0 },
  { MULTIPAGE_CreateIndirect, "Multipage", ID_MULTIPAGE, 5, 20, 220, 240, 0, 0x0, 0 },  
};

static const GUI_WIDGET_CREATE_INFO _aDialogSystemInformation[] = {
  { WINDOW_CreateIndirect, "System Information", 0, 0,   0, 220, 240, FRAMEWIN_CF_MOVEABLE },
  { TEXT_CreateIndirect, "Core : STM32429", ID_TEXT_BOARD, 10, 35, 200, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Author: HouJun", ID_TEXT_CORE, 10, 55, 200, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "CPU Speed :168MHz", ID_TEXT_CPU, 10, 75, 200, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "OS:RT-Thread", ID_TEXT_VERSION, 10, 95, 200, 20, 0, 0x0, 0 },
  { IMAGE_CreateIndirect, "Image", ID_IMAGE_LOGO, 90, 130, 40, 20, 0, 0, 0 },  
  { TEXT_CreateIndirect, "Copyright (c) STMicroelectronics 2019", ID_TEXT_COPYRIGHT, 10, 160, 200, 20, 0, 0x0, 0 },  
};

static const GUI_WIDGET_CREATE_INFO _aDialogGeneralSettings[] = {
  { WINDOW_CreateIndirect, "GeneralSettings", 0, 0,   0, 220, 240, FRAMEWIN_CF_MOVEABLE },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_WIFI,165,35, 45, 25, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "WIFI", ID_TEXT_WIFI, 5, 41, 80, 25, 0, 0x0, 0 },	
	{ BUTTON_CreateIndirect, "Button", ID_BUTTON_Temp,165,70, 45, 25, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Temp Numerical display", ID_TEXT_Temp, 5, 70, 110, 25, 0, 0x0, 0 },	
};

/*************************************************************
*函数功能:获取系统设置APP设置保存结构体地址
*传入参数:无
*返回值  :结构体变量地址
**************************************************************/
SystemConfig * GetSystemConfigStruct(void)
{
	return &syssetting;
}

/*************************************************************
*函数功能:获取系统设置APP设置保存结体变量初始化
*传入参数:无
*返回值  :无
**************************************************************/
void InitSystemConfigStruct(void)
{
	memset(&syssetting,0,sizeof(syssetting));
}

/**************************************************************
*函数功能：设置复用回调函数
*传入参数：
*返回值  ：
**************************************************************/
static void _cbSystemInformation(WM_MESSAGE * pMsg) 
{
  WM_HWIN hItem;
  char str[40];
  hItem = pMsg->hWin;
  switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:

			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_BOARD);//Core: STM32F429
			TEXT_SetFont(hItem, GUI_FONT_13_ASCII);
			TEXT_SetTextColor(hItem, GUI_DARKGRAY);

			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CORE);//Author
			TEXT_SetFont(hItem, GUI_FONT_13_ASCII);
			TEXT_SetTextColor(hItem, GUI_RED);

			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CPU);//CPU Speed : 168MHz' */
			TEXT_SetFont(hItem, GUI_FONT_13_ASCII);
			TEXT_SetTextColor(hItem, GUI_DARKGRAY);

			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_VERSION);//OS:RT-Thread
			TEXT_SetFont(hItem, GUI_FONT_13_ASCII);
			TEXT_SetTextColor(hItem, GUI_DARKGRAY);

			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_COPYRIGHT);//corpright
			TEXT_SetFont(hItem, GUI_FONT_13_ASCII);
			TEXT_SetTextColor(hItem, GUI_DARKGRAY);
			
	//    /* Set ST Logo */
	//    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_LOGO);//logo
	//    IMAGE_SetBitmap(hItem, &bmSTLogo40x20);
			
			break;
		default:
			WM_DefaultProc(pMsg);
  }
}


/**************************************************************
*函数功能：设置复用页回调函数
*传入参数：
*返回值  ：
**************************************************************/
static void _cbGeneralSettings(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
  int     NCode;
  int     Id;
  switch (pMsg->MsgId)
	{
		case WM_INIT_DIALOG:
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
			TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_WIFI);
			BUTTON_SetBitmapEx(hItem,0,syssetting.Button_Wifi?&bmon:&bmoff,0,0);
			BUTTON_SetText(hItem, "");
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Temp);
			TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_Temp);
			BUTTON_SetBitmapEx(hItem,0,syssetting.Button_Temp?&bmon:&bmoff,0,0);
			BUTTON_SetText(hItem, "");
			break;
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id) 
			{
				case ID_BUTTON_WIFI: 
					hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_WIFI);
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							syssetting.Button_Wifi=~syssetting.Button_Wifi;
							BUTTON_SetBitmapEx(hItem,0,syssetting.Button_Wifi?&bmon:&bmoff,0,0);
							break;
					}
					break;
					
				case ID_BUTTON_Temp: 
					hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_Temp);
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							syssetting.Button_Temp=~syssetting.Button_Temp;
							BUTTON_SetBitmapEx(hItem,0,syssetting.Button_Temp?&bmon:&bmoff,0,0);
							break;
					}
					break;
					
				}
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
  }
}

/**************************************************************
*函数功能：设置APP回调函数
*传入参数：
*返回值  ：
**************************************************************/
static void _cbDialog(WM_MESSAGE * pMsg) 
{
  WM_HWIN hItem, hDialog;
  
  switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:

			hItem = pMsg->hWin;
			FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT, 0);   //右上角关闭button 
			FRAMEWIN_AddMaxButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //添加最大化按钮
			FRAMEWIN_AddMinButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //添加最小化按钮	
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_INFO);//背景图片
			IMAGE_SetBitmap(hItem, &bminfo);
			hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIPAGE);

			hDialog = GUI_CreateDialogBox(_aDialogSystemInformation, 
																		GUI_COUNTOF(_aDialogSystemInformation), 
																		&_cbSystemInformation, WM_UNATTACHED, 0, 0);
			MULTIPAGE_AddPage(hItem, hDialog, "System Information");
			
			hDialog = GUI_CreateDialogBox(_aDialogGeneralSettings, 
																		GUI_COUNTOF(_aDialogGeneralSettings), 
																		&_cbGeneralSettings, WM_UNATTACHED, 0, 0);
			MULTIPAGE_AddPage(hItem, hDialog, "GeneralSettings");
			

			MULTIPAGE_SelectPage(hItem, 0);  
			
			break;
		 
		default:
			WM_DefaultProc(pMsg);
			break;
  }
}

/**************************************************************
*函数功能：设置APP
*传入参数：当前窗口
*返回值  ：
**************************************************************/
void CreateSystemSetting(WM_HWIN hWin)
{
  GUI_CreateDialogBox(_aDialog, GUI_COUNTOF(_aDialog), _cbDialog,hWin, 0, 26);
}


