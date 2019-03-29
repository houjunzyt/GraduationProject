#include "temperature.h"

static WM_HWIN  hTemp = 0;
static GRAPH_DATA_Handle hDataTemp1 = 0;
static GRAPH_DATA_Handle hDataTemp2 = 0;
static GRAPH_DATA_Handle hDataTemp3 = 0;
static GRAPH_DATA_Handle hDataTemp4 = 0;
static GRAPH_SCALE_Handle hScale = 0;

static const GUI_WIDGET_CREATE_INFO _aTemperatureDialogCreate[] = 
{
  { FRAMEWIN_CreateIndirect, "Temperature", ID_FRAMEWIN_Temp, 10, 81, 220, 180, 0, 0x0, 0 },
  { GRAPH_CreateIndirect, "", ID_GRAPH_Temp, 5, 9, 200, 130, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Hide", ID_BUTTON_HIDE, 70, 142, 80, 30, 0, 0x0, 0 },
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
			GRAPH_DATA_YT_AddValue(hDataTemp1 , 15);
			GRAPH_DATA_YT_AddValue(hDataTemp2 , 20);
			GRAPH_DATA_YT_AddValue(hDataTemp3 , 25);
			GRAPH_DATA_YT_AddValue(hDataTemp4 , 37);
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
