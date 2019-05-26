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

/***************************************************************
*�������ܣ���ȡͼƬID
*���������ID�ꡢ��Ŵ�С�Ŀռ�
*����ֵ  ��ͼƬָ��
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
*�������ܣ�����ͼ����ʽ�ص�����
*���������
*����ֵ  ��
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
			FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT,2); //���Ͻǹر�button    
			FRAMEWIN_AddMaxButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //�����󻯰�ť
			FRAMEWIN_AddMinButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //�����С����ť	
			FRAMEWIN_SetTitleHeight(hItem,18);//���ø߶�
			hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_T0);
			pData = _GetImageById(ID_IMAGE_0_IMAGE_0, &FileSize);
			IMAGE_SetPNG(hItem, pData, FileSize);

			hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_T1);//���ͼƬ
			pData = _GetImageById(ID_IMAGE_1_IMAGE_0, &FileSize);
			IMAGE_SetPNG(hItem, pData, FileSize);

			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_T0);
			TEXT_SetText(hItem, "T1:");
			 
			break;
		
		case WM_TIMER:
		  hItem=pMsg->hWin;
			hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_T4);//��ʾ�¶�ֵ
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
*�������ܣ��¶�APP����ͼ����ʽ
*���������
*����ֵ  ��
********************************************************/
WM_HWIN CreateTempNum(void) 
{
  WM_HWIN hWin;
	static WM_HTIMER hTimerTime; 
  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  hTimerTime = WM_CreateTimer(WM_GetClientWindow(hWin),0,1000,0); //����һ����ʱ�� 
	return hWin;
}


