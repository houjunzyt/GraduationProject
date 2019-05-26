#include "switch.h"

Switchconfig Switch={0};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
  { FRAMEWIN_CreateIndirect, "Switch", ID_FRAMEWIN_Switch, 1, 53, 240, 240, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Air_conditioner", ID_TEXT_Air, 26, 37, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Water heater", ID_TEXT_Wat, 27, 77, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Washing", ID_TEXT_Was, 27, 116, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Plug", ID_TEXT_Plu, 28, 154, 80, 20, 0, 0x0, 0 },

	{ BUTTON_CreateIndirect, "Button", ID_BUTTON_Air, 138, 35, 45, 25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_Wat, 138, 75, 45, 25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_Was, 138, 114, 45, 25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_Plu, 139, 152, 45, 25, 0, 0x0, 0 },
};

static void _cbSwitchDialog(WM_MESSAGE * pMsg) 
{
  WM_HWIN hItem;
	int     NCode;
  int     Id;
  switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
			hItem = pMsg->hWin;
			FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT, 0);   //右上角关闭button 
			FRAMEWIN_AddMaxButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //添加最大化按钮
			FRAMEWIN_AddMinButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //添加最小化按钮	
			FRAMEWIN_SetTitleHeight(hItem,18);//设置高度
			//text初始化
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Air);
			TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Wat);
			TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);

			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Was);
			TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);

			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Plu);
			TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			//按键初始化
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_Air);
			BUTTON_SetBitmapEx(hItem,0,Switch.Air_conditioner?&bmon:&bmoff,0,0);
			BUTTON_SetText(hItem, "");
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_Wat);
			BUTTON_SetBitmapEx(hItem,0,Switch.Water_heater?&bmon:&bmoff,0,0);
			BUTTON_SetText(hItem, "");
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_Was);
			BUTTON_SetBitmapEx(hItem,0,Switch.Washing?&bmon:&bmoff,0,0);
			BUTTON_SetText(hItem, "");
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_Plu);
			BUTTON_SetBitmapEx(hItem,0,Switch.Plug?&bmon:&bmoff,0,0);
			BUTTON_SetText(hItem, "");
	
			break;
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id) 
			{
				case ID_BUTTON_Air: // Notifications sent by 'Button'
					hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_Air);
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							Switch.Air_conditioner=~Switch.Air_conditioner;
							BUTTON_SetBitmapEx(hItem,0,Switch.Air_conditioner?&bmon:&bmoff,0,0);
							break;
					}
					break;
				case ID_BUTTON_Wat: // Notifications sent by 'Button'
					hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_Wat);
					switch(NCode) 	
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							Switch.Water_heater=~Switch.Water_heater;
							BUTTON_SetBitmapEx(hItem,0,Switch.Water_heater?&bmon:&bmoff,0,0);
							break;
					}
					break;
				case ID_BUTTON_Was: // Notifications sent by 'Button'
					hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_Was);
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							Switch.Washing=~Switch.Washing;
							BUTTON_SetBitmapEx(hItem,0,Switch.Washing?&bmon:&bmoff,0,0);
							break;
					}
					break;
				case ID_BUTTON_Plu: // Notifications sent by 'Button'
					hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_Plu);
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							Switch.Plug=~Switch.Plug;
							BUTTON_SetBitmapEx(hItem,0,Switch.Plug?&bmon:&bmoff,0,0);
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

WM_HWIN CreateSwitch(void) 
{
  WM_HWIN hWin;
  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbSwitchDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

