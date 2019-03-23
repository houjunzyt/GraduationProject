#include "systemset.h"

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
//  { WINDOW_CreateIndirect, "General Settings", 0, 0,   0, 220, 240, FRAMEWIN_CF_MOVEABLE },
//  { CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_SPRITES, 26, 30, 147, 20, 0, 0x0, 0 },
//  { CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_CPU180, 26, 60, 215, 20, 0, 0x0, 0 },
//  { CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_FLEXSKIN, 26, 90, 147, 20, 0, 0x0, 0 },
//  { TEXT_CreateIndirect, "[!! Restart the demonstration to apply", ID_TEXT_WARNING0, 6, 180, 210, 20, 0, 0x0, 0 }, 
//  { TEXT_CreateIndirect, "    the new settings !!]", ID_TEXT_WARNING1, 6, 200, 210, 20, 0, 0x0, 0 },
};



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

			/* ST Copyright */
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
  hItem = pMsg->hWin;
  switch (pMsg->MsgId) 
	{
    
		case WM_INIT_DIALOG:    
    
//		settings.d32 = k_BkupRestoreParameter(CALIBRATION_GENERAL_SETTINGS_BKP);

//		/* Initialization of 'Checkbox' (sprite field) */
//		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_SPRITES);
//		CHECKBOX_SetText(hItem, "Enable Sprites");
//		CHECKBOX_SetState(hItem, settings.b.enable_sprite);
//			
//		/* Initialization of 'Checkbox' (CPU field) */
//		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_CPU180);
//		CHECKBOX_SetText(hItem, "Run CPU at 180 MHz");
//		CHECKBOX_SetState(hItem, settings.b.use_180Mhz);      
//		/* Initialization of 'Checkbox' (flex skin field) */
//		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_FLEXSKIN);
//		CHECKBOX_SetText(hItem, "Disable Flex skin");
//		CHECKBOX_SetState(hItem, settings.b.disable_flex_skin);       

//		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_WARNING0);
//		TEXT_SetTextColor(hItem, GUI_DARKRED);

//		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_WARNING1);
//		TEXT_SetTextColor(hItem, GUI_DARKRED);
			break;
    
		case WM_DELETE:  
//    /* Save Setting before delete settings frame */
//    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_SPRITES);
//    settings.b.enable_sprite = CHECKBOX_IsChecked(hItem);
//    
//    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_CPU180);
//    settings.b.use_180Mhz = CHECKBOX_IsChecked(hItem);
//    
//    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_FLEXSKIN);
//    settings.b.disable_flex_skin = CHECKBOX_IsChecked(hItem);
//    
//    tmp = k_BkupRestoreParameter(CALIBRATION_GENERAL_SETTINGS_BKP);
//	
//    /* check if new settings have to be saved */
//    if(settings.d32 != tmp)
//    {
//      k_BkupSaveParameter(CALIBRATION_GENERAL_SETTINGS_BKP, settings.d32); 
//    }
			break;    
    
		default:
			WM_DefaultProc(pMsg);
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
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_INFO);//背景图片
			IMAGE_SetBitmap(hItem, &bminfo);
			hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIPAGE);

			hDialog = GUI_CreateDialogBox(_aDialogSystemInformation, 
																		GUI_COUNTOF(_aDialogSystemInformation), 
																		&_cbSystemInformation, WM_UNATTACHED, 0, 0);
			MULTIPAGE_AddPage(hItem, hDialog, "System Information");
			
		//    hDialog = GUI_CreateDialogBox(_aDialogGeneralSettings, 
		//                                  GUI_COUNTOF(_aDialogGeneralSettings), 
		//                                  &_cbGeneralSettings, WM_UNATTACHED, 0, 0);
		//    MULTIPAGE_AddPage(hItem, hDialog, "General Settings");
			

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


