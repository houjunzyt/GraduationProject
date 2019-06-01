#include "weather.h"
#include "rtthread.h"

extern uint8_t weatherFlag,tempL,tempH,C,L;
extern const uint8_t weatherName[3][7][20];
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
  { FRAMEWIN_CreateIndirect, "Weather", ID_FRAMEWIN_Weather, 1, 53, 240, 240, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Weather", ID_TEXT_WEA, 26, 37, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Temp Range", ID_TEXT_WTE, 27, 77, 80, 20, 0, 0x0, 0 },
	
	{ TEXT_CreateIndirect, "", ID_TEXT_wea, 138, 35, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "", ID_TEXT_wte, 138, 75, 80, 20, 0, 0x0, 0 },

};

static void _cbWeatherDialog(WM_MESSAGE * pMsg) 
{
  WM_HWIN hItem;
	int     NCode;
  int     Id;
	uint8_t Range[10];
  switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
			hItem = pMsg->hWin;
			FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT, 0);   //右上角关闭button 
			FRAMEWIN_AddMaxButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //添加最大化按钮
			FRAMEWIN_AddMinButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //添加最小化按钮	
			FRAMEWIN_SetTitleHeight(hItem,18);//设置高度
			//text初始化
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_WEA);
			TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_WTE);
			TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);

			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_wea);
			TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);

			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_wte);
			TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			WM_InvalidateWindow(pMsg->hWin);
			WM_RestartTimer(pMsg->Data.v, 1000);
			break;
		case  WM_TIMER:
			
			if(weatherFlag)
			{
				hItem=pMsg->hWin;
				hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_wea);//显示天气
				TEXT_SetFont(hItem, GUI_FONT_8_ASCII);
				TEXT_SetText(hItem,(char *)weatherName[C][L]);
				rt_kprintf("wea:%s\n",weatherName[C][L]);
				TEXT_SetTextAlign(hItem,GUI_TA_HCENTER|GUI_TA_VCENTER);
				
				hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_wte);//显示温度范围
				TEXT_SetFont(hItem, GUI_FONT_8_ASCII);
				sprintf((char *)Range,"%d -%d C",tempL,tempH);
				TEXT_SetText(hItem,(char *)Range);
				TEXT_SetTextAlign(hItem,GUI_TA_HCENTER|GUI_TA_VCENTER);
			}
			WM_InvalidateWindow(pMsg->hWin);
			WM_RestartTimer(pMsg->Data.v, 1000);
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
  }
}


WM_HWIN CreatWeather(void) 
{
	static WM_HTIMER hTimerTime; 
  WM_HWIN hWin;
  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbWeatherDialog, WM_HBKWIN, 0, 0);
	hTimerTime = WM_CreateTimer(WM_GetClientWindow(hWin),0,1000,0); //创建一个定时器 
  return hWin;
}

