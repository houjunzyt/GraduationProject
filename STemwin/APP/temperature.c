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
*�������ܣ��¶�ֵ����
*���������
*����ֵ  ��
********************************************************/
static void _cbTempWindow(WM_MESSAGE * pMsg) 
{
  static WM_HTIMER hTimerTime; 
  switch (pMsg->MsgId) 
  {
		case WM_CREATE:
			hTimerTime = WM_CreateTimer(pMsg->hWin, 0, 400, 0); //����һ����ʱ��       
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
*�������ܣ��¶�app���ڻص�����
*���������
*����ֵ  ��
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

			FRAMEWIN_SetTitleVis(pMsg->hWin,0);//���ضԻ���
			hGraph = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_Temp);
			hDataTemp1 = GRAPH_DATA_YT_Create(GUI_LIGHTGREEN, 500, 0, 10);//��������ʹ�õ���ɫ��������������ֵ��ָ��������ݵ�ָ�롢��ӵ���������Ŀ
			hDataTemp2 = GRAPH_DATA_YT_Create(GUI_LIGHTBLUE, 500, 0, 10);//��������ʹ�õ���ɫ��������������ֵ��ָ��������ݵ�ָ�롢��ӵ���������Ŀ		
			hDataTemp3 = GRAPH_DATA_YT_Create(GUI_LIGHTRED, 500, 0, 10);//��������ʹ�õ���ɫ��������������ֵ��ָ��������ݵ�ָ�롢��ӵ���������Ŀ	
			hDataTemp4 = GRAPH_DATA_YT_Create(GUI_DARKYELLOW,500, 0, 10);//��������ʹ�õ���ɫ��������������ֵ��ָ��������ݵ�ָ�롢��ӵ���������Ŀ	
			GRAPH_SetGridVis(hGraph, 1);//�����������
			GRAPH_SetBorder(hGraph, 30, 2, 2, 2); //����ͼ��ı߿����ԣ������������ߡ��ұߡ��ϱߡ��±ߵ�����
			GRAPH_AttachData(hGraph, hDataTemp1);//������ݶ���
			GRAPH_AttachData(hGraph, hDataTemp2);//������ݶ���
			GRAPH_AttachData(hGraph, hDataTemp3);//������ݶ���
			GRAPH_AttachData(hGraph, hDataTemp4);//������ݶ���
			hScale = GRAPH_SCALE_Create(20, GUI_TA_RIGHT, GRAPH_SCALE_CF_VERTICAL,25);//�����̶ȶ������/���ߵ�λ�á����뷽ʽ���ң����̶ȷ��򣨴�ֱ�����̶��߼��
			GRAPH_AttachScale(hGraph, hScale); //��ӿ̶ȶ��� 
			GRAPH_SCALE_SetTextColor(hScale, GUI_YELLOW);//���ÿ̶ȵ��ı���ɫ
			GRAPH_SetGridDistX(hGraph,25);//����������
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
*�������ܣ��¶�APP����
*���������
*����ֵ  ��
********************************************************/
void CreateTempAPP(WM_HWIN hWin)
{
	if(hTemp == 0)//����һ��ͼ��������
	{
		hTemp = GUI_CreateDialogBox(_aTemperatureDialogCreate, 
																			 GUI_COUNTOF(_aTemperatureDialogCreate), 
																			 _cbTemperatureDialog, //����
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
