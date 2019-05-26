#include "temperature.h"
#include "temperaturebackground.c"
#include <stdio.h>

static WM_HWIN  hTemp = 0;
static GRAPH_DATA_Handle hDataTemp1 = 0;
static GRAPH_DATA_Handle hDataTemp2 = 0;
static GRAPH_DATA_Handle hDataTemp3 = 0;
static GRAPH_DATA_Handle hDataTemp4 = 0;
static GRAPH_SCALE_Handle hScale = 0;

extern short DS18B20_temperature1,DS18B20_temperature2,DS18B20_temperature3,DS18B20_temperature4;

static const GUI_WIDGET_CREATE_INFO _aTemperatureDialogCreate[] = 
{
  { FRAMEWIN_CreateIndirect, "Temperature", ID_FRAMEWIN_Temp, 10, 81, 220, 180, 0, 0x0, 0 },
  { GRAPH_CreateIndirect, "", ID_GRAPH_Temp, 5, 9, 200, 130, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Hide", ID_BUTTON_HIDE, 70, 142, 80, 30, 0, 0x0, 0 },
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Temperature", ID_FRAMEWIN_T0, 0, 26, 240, 294, 0, 0x0, 0 },
  { IMAGE_CreateIndirect, "Image", ID_IMAGE_T0, 0, 0, 230, 297, 0, 0, 0 },
  { IMAGE_CreateIndirect, "Image", ID_IMAGE_T1, 16, 98, 58, 58, 0, 0, 0 },
  { TEXT_CreateIndirect, "T1:", ID_TEXT_T0, 103, 45, 30, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "T2:", ID_TEXT_T1, 102, 92, 21, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "T3:", ID_TEXT_T2, 102, 138, 16, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "T4:", ID_TEXT_T3, 102, 185, 26, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_T4, 145, 45, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_T5, 146, 91, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_T6, 148, 138, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_T7, 147, 185, 80, 20, 0, 0x0, 0 },
};
/********************************************************
*函数功能：温度值绘制
*传入参数：
*返回值  ：
********************************************************/
static void _cbTempWindow(WM_MESSAGE * pMsg) 
{
  static WM_HTIMER hTimerTime; 
  switch (pMsg->MsgId) 
  {
		case WM_CREATE:
			hTimerTime = WM_CreateTimer(pMsg->hWin, 0, 400, 0); //创建一个定时器       
			break;
			
		case WM_TIMER:
			GRAPH_DATA_YT_AddValue(hDataTemp1 , DS18B20_temperature1/10);
			GRAPH_DATA_YT_AddValue(hDataTemp2 , DS18B20_temperature2/10);
			GRAPH_DATA_YT_AddValue(hDataTemp3 , DS18B20_temperature3/10);
			GRAPH_DATA_YT_AddValue(hDataTemp4 , DS18B20_temperature4/10);
			WM_InvalidateWindow(pMsg->hWin);
			WM_RestartTimer(pMsg->Data.v, 400);
			break; 
			
		case WM_DELETE:
			WM_DeleteTimer(hTimerTime);
			break;
			
		default:
			WM_DefaultProc(pMsg);
  }
}

/********************************************************
*函数功能：温度app串口回调函数
*传入参数：
*返回值  ：
********************************************************/
static void _cbTemperatureDialog(WM_MESSAGE * pMsg) 
{
  int NCode;
  int Id;
  WM_HWIN hGraph;

  GUI_RECT Rect;

  switch (pMsg->MsgId) 
  {
		case WM_INIT_DIALOG:

			FRAMEWIN_SetTitleVis(pMsg->hWin,0);//隐藏对话框
			hGraph = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_Temp);
			hDataTemp1 = GRAPH_DATA_YT_Create(GUI_LIGHTGREEN, 500, 0, 10);//绘制数据使用的颜色、数据项的最大数值、指向对象数据的指针、添加的数据项数目
			hDataTemp2 = GRAPH_DATA_YT_Create(GUI_LIGHTBLUE, 500, 0, 10);//绘制数据使用的颜色、数据项的最大数值、指向对象数据的指针、添加的数据项数目		
			hDataTemp3 = GRAPH_DATA_YT_Create(GUI_LIGHTRED, 500, 0, 10);//绘制数据使用的颜色、数据项的最大数值、指向对象数据的指针、添加的数据项数目	
			hDataTemp4 = GRAPH_DATA_YT_Create(GUI_DARKYELLOW,500, 0, 10);//绘制数据使用的颜色、数据项的最大数值、指向对象数据的指针、添加的数据项数目	
			GRAPH_SetGridVis(hGraph, 1);//启用网格绘制
			GRAPH_SetBorder(hGraph, 30, 2, 2, 2); //设置图标的边框属性：句柄、距离左边、右边、上边、下边的像素
			GRAPH_AttachData(hGraph, hDataTemp1);//添加数据对象
			GRAPH_AttachData(hGraph, hDataTemp2);//添加数据对象
			GRAPH_AttachData(hGraph, hDataTemp3);//添加数据对象
			GRAPH_AttachData(hGraph, hDataTemp4);//添加数据对象
			hScale = GRAPH_SCALE_Create(20, GUI_TA_RIGHT, GRAPH_SCALE_CF_VERTICAL,25);//创建刻度对象：左边/顶边的位置、对齐方式（右）、刻度方向（垂直）、刻度线间距
			GRAPH_AttachScale(hGraph, hScale); //添加刻度对象 
			GRAPH_SCALE_SetTextColor(hScale, GUI_YELLOW);//设置刻度的文本颜色
			GRAPH_SetGridDistX(hGraph,25);//设置网格间距
			GRAPH_SetGridDistY(hGraph,25);
			WM_GetClientRect(&Rect);
			WM_CreateWindowAsChild(230, 0, 10, 10, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbTempWindow , 0); 
			break;
			
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id) {
				
			case ID_BUTTON_HIDE: /* Notifications sent by Hide Button */
				switch(NCode) 
				{
				case WM_NOTIFICATION_RELEASED:
					
					if(hTemp != 0)
					{
						WM_HideWindow(hTemp);
					}
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

/********************************************************
*函数功能：温度APP创建
*传入参数：
*返回值  ：
********************************************************/
void CreateTempAPP(WM_HWIN hWin)
{
	if(hTemp == 0)//创建一个图形组件框架
	{
		hTemp = GUI_CreateDialogBox(_aTemperatureDialogCreate, 
																			 GUI_COUNTOF(_aTemperatureDialogCreate), 
																			 _cbTemperatureDialog, //绘制
																			 hWin, 
																			 0, 
																			 0);
	}
	else
	{
		WM_ShowWindow(hTemp);
		WM_BringToTop(hTemp);
	}
}

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
  }
  return NULL;
}

/********************************************************
*函数功能：创建图形形式回调函数
*传入参数：
*返回值  ：
********************************************************/
static void _cbDialog(WM_MESSAGE * pMsg) 
{
  const void * pData;
  WM_HWIN      hItem;
  U32          FileSize;
	uint8_t      tem[6];
  switch (pMsg->MsgId) 
	{

		case WM_INIT_DIALOG:
			hItem = pMsg->hWin;
			FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT,2); //右上角关闭button    
			FRAMEWIN_AddMaxButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //添加最大化按钮
			FRAMEWIN_AddMinButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //添加最小化按钮	
			FRAMEWIN_SetTitleHeight(hItem,18);//设置高度
			hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_T0);
			pData = _GetImageById(ID_IMAGE_0_IMAGE_0, &FileSize);
			IMAGE_SetPNG(hItem, pData, FileSize);

			hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_T1);//添加图片
			pData = _GetImageById(ID_IMAGE_1_IMAGE_0, &FileSize);
			IMAGE_SetPNG(hItem, pData, FileSize);

			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_T0);
			TEXT_SetText(hItem, "T1:");
			 
			break;
		
		case WM_TIMER:
		  hItem=pMsg->hWin;
			hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_T4);//显示温度值
			TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
			sprintf((char *)tem,"%d.%dC",DS18B20_temperature1/10,DS18B20_temperature1%10);
			TEXT_SetText(hItem,(char *)tem);
			TEXT_SetTextAlign(hItem,GUI_TA_HCENTER|GUI_TA_VCENTER);

		  hItem=pMsg->hWin;
			hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_T5);
			TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
			sprintf((char *)tem,"%d.%dC",DS18B20_temperature2/10,DS18B20_temperature2%10);
			TEXT_SetText(hItem,(char *)tem);
			TEXT_SetTextAlign(hItem,GUI_TA_HCENTER|GUI_TA_VCENTER);
		
			hItem=pMsg->hWin;
			hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_T6);
			TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
			sprintf((char *)tem,"%d.%dC",DS18B20_temperature3/10,DS18B20_temperature3%10);
			TEXT_SetText(hItem,(char *)tem);
			TEXT_SetTextAlign(hItem,GUI_TA_HCENTER|GUI_TA_VCENTER);
		
		
			hItem=pMsg->hWin;
			hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_T7);
			TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
			sprintf((char *)tem,"%d.%dC",DS18B20_temperature4/10,DS18B20_temperature4%10);
			TEXT_SetText(hItem,(char *)tem);
			TEXT_SetTextAlign(hItem,GUI_TA_HCENTER|GUI_TA_VCENTER);
		
			WM_InvalidateWindow(pMsg->hWin);
			WM_RestartTimer(pMsg->Data.v, 1000);
		
			break; 
			
		default:
			WM_DefaultProc(pMsg);
			break;
  }
}

/********************************************************
*函数功能：温度APP创建图形形式
*传入参数：
*返回值  ：
********************************************************/
WM_HWIN CreateTempNum(void) 
{
  WM_HWIN hWin;
	static WM_HTIMER hTimerTime; 
  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  hTimerTime = WM_CreateTimer(WM_GetClientWindow(hWin),0,1000,0); //创建一个定时器 
	return hWin;
}


