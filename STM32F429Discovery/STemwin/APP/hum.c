#include "hum.h"
#include "humbackground.c"

extern rt_uint8_t DHT11_temperature;  	    
extern rt_uint8_t DHT11_humidity;
extern uint16_t ADC_ConvertedValue;
			
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Humidity", ID_FRAMEWIN_0, 2, 53, 240, 240, 0, 0x0, 0 },
  { IMAGE_CreateIndirect, "Image", ID_IMAGE_H0, 0, 0, 230, 292, 0, 0, 0 },
  { IMAGE_CreateIndirect, "Image", ID_IMAGE_H1, 42, 33, 60, 61, 0, 0, 0 },
  { IMAGE_CreateIndirect, "Image", ID_IMAGE_H2, 43, 125, 53, 60, 0, 0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_H0, 136, 49, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_H1, 136, 150, 80, 20, 0, 0x0, 0 },
};

/***************************************************************
*函数功能：获取图片ID
*传入参数：ID宏、存放大小的空间
*返回值  ：图片指针
***************************************************************/
static const void * _GetImageById(U32 Id, U32 * pSize) 
{
  switch (Id) 
	{
		case ID_IMAGE_0_IMAGE_0:
			*pSize = sizeof(_acImage_0);
			return (const void *)_acImage_0;
		case ID_IMAGE_1_IMAGE_0:
			*pSize = sizeof(_acImage_1);
			return (const void *)_acImage_1;
		case ID_IMAGE_2_IMAGE_0:
			*pSize = sizeof(_acImage_2);
			return (const void *)_acImage_2;
  }
  return NULL;
}

/********************************************************
*函数功能：湿度APP创建回调函数
*传入参数：
*返回值  ：
********************************************************/
static void _cbDialog(WM_MESSAGE * pMsg) 
{
  const void * pData;
  WM_HWIN      hItem;
  U32          FileSize;
	uint8_t      DHT11[50];
  switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
			hItem = pMsg->hWin;
			FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT,2); //右上角关闭button    
			FRAMEWIN_AddMaxButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //添加最大化按钮
			FRAMEWIN_AddMinButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //添加最小化按钮	
			FRAMEWIN_SetTitleHeight(hItem,18);//设置高度		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_H0);
			pData = _GetImageById(ID_IMAGE_0_IMAGE_0, &FileSize);
			IMAGE_SetPNG(hItem, pData, FileSize);

			hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_H1);
			pData = _GetImageById(ID_IMAGE_1_IMAGE_0, &FileSize);
			IMAGE_SetPNG(hItem, pData, FileSize);

			hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_H2);
			pData = _GetImageById(ID_IMAGE_2_IMAGE_0, &FileSize);
			IMAGE_SetPNG(hItem, pData, FileSize);
			break;
		case WM_TIMER:
			hItem=pMsg->hWin;
			hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_H0);//显示PM
			TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
			sprintf((char *)DHT11,"%d",ADC_ConvertedValue);	
			TEXT_SetText(hItem,DHT11);
			TEXT_SetTextAlign(hItem,GUI_TA_HCENTER|GUI_TA_VCENTER);
		
			hItem=pMsg->hWin;
			hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_H1);//显示湿度
			TEXT_SetFont(hItem, GUI_FONT_16_ASCII);
			TEXT_SetTextColor(hItem,GUI_WHITE);
			sprintf((char *)DHT11,"%d \%RH %d °C",DHT11_humidity,DHT11_temperature);
			TEXT_SetText(hItem,(const char *)DHT11);
			TEXT_SetTextAlign(hItem,GUI_TA_HCENTER|GUI_TA_VCENTER);
		
			WM_InvalidateWindow(pMsg->hWin);
			WM_RestartTimer(pMsg->Data.v, 400);
		default:
			WM_DefaultProc(pMsg);
			break;
  }
}

/********************************************************
*函数功能：湿度APP创建
*传入参数：
*返回值  ：
********************************************************/
WM_HWIN CreateHumApp(void) 
{
  WM_HWIN hWin;
	static WM_HTIMER hTimerTime; 
  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	hTimerTime = WM_CreateTimer(WM_GetClientWindow(hWin),0,400,0); //创建一个定时器 
  return hWin;
}
