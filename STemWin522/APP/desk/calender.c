#include "calender.h"

#define  PI                                3.14
#define  AA_FACTOR                         3

#define X0                                 49
#define Y0                                 48

static const GUI_WIDGET_CREATE_INFO _aDialog[] = 
{
  
  { FRAMEWIN_CreateIndirect, "Calender", ID_FRAMEWIN_INFO, 0, 0, 240, 294, 0, 0x64, 0 },
  { MULTIPAGE_CreateIndirect, "Multipage", ID_MULTIPAGE, 5, 20, 220, 240, 0, 0x0, 0 },  
};

static const GUI_WIDGET_CREATE_INFO _aDialogDateSettings[] = {
  { WINDOW_CreateIndirect, "Date Settings", 0, 0,   0, 220, 240, FRAMEWIN_CF_MOVEABLE },
  { BUTTON_CreateIndirect, "Apply settings", ID_BUTTON_APPLYDATESETTINGS, 50, 182, 120, 35, 0, 0x0, 0 },
};

static const GUI_WIDGET_CREATE_INFO _aDialogClockSettings[] = {
  { WINDOW_CreateIndirect, "Clock Settings", 0, 0,   0, 220, 240, FRAMEWIN_CF_MOVEABLE },
  { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_HOUR, 28, 123, 40, 30, 0, 0x0, 0 },  
  { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_MINUTE, 88, 123, 40, 30, 0, 0x0, 0 },
  { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_SEC, 151, 123, 40, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Apply settings", ID_BUTTON_APPLYCLKSETTINGS, 50, 180, 120, 37, 0, 0x0, 0 },
};

static const GUI_POINT aPoints[3][4] = {
   
  /* Hour Needle */
  {{ 0 * AA_FACTOR, 2 * AA_FACTOR},
   {-1 * AA_FACTOR,-4 * AA_FACTOR},
   { 0 * AA_FACTOR,-6 * AA_FACTOR},
   { 1 * AA_FACTOR,-4 * AA_FACTOR}},

  /* Min Needle */
  {{ 0 * AA_FACTOR, 2 * AA_FACTOR},
   {-1 * AA_FACTOR,-2 * AA_FACTOR},
   { 0 * AA_FACTOR,-8 * AA_FACTOR},
   { 1 * AA_FACTOR,-2 * AA_FACTOR}},
   /* Sec Needle */
  {{0 * AA_FACTOR, 1 * AA_FACTOR},
   { 1 * AA_FACTOR, 1 * AA_FACTOR},
   { 1 * AA_FACTOR,-34 * AA_FACTOR},
   {0 * AA_FACTOR,-34 * AA_FACTOR}},
};


GUI_POINT aPointsDest[3][4];

/**************************************************************
*函数功能：时间钟的指针函数
*传入参数：
*返回值  ：
**************************************************************/
static void DrawNeedle(uint32_t index, uint16_t x0, uint16_t y0) 
{
  /* draw Needles */
  if(index == 2)
  {
    GUI_SetColor(GUI_RED);
    GUI_AA_FillPolygon(aPointsDest[index], 4, AA_FACTOR * x0, AA_FACTOR * y0);
  }
  else 
  {
    GUI_SetColor(GUI_LIGHTBLUE);
    GUI_AA_FillPolygon(aPointsDest[index], 4, AA_FACTOR * x0, AA_FACTOR * y0);
  }
}

/**************************************************************
*函数功能：日历APP时间备份
*传入参数：
*返回值  ：
**************************************************************/
static void GUI_UpdateClock (uint16_t x0, uint16_t y0, uint8_t hour, uint8_t min, uint8_t sec)
{
  int8_t i = 0;
  int32_t SinHQ, CosHQ ,a = 0;
  uint16_t xPos, yPos;
    
  GUI_AA_EnableHiRes();
  GUI_AA_SetFactor(AA_FACTOR);
  
  GUI_SetColor(GUI_LIGHTGRAY);   
  GUI_AA_DrawArc(AA_FACTOR * x0, AA_FACTOR * y0, AA_FACTOR * 39, AA_FACTOR * 39, 0, 360);  
  GUI_SetColor(GUI_GRAY);  
  GUI_AA_DrawArc(AA_FACTOR * x0, AA_FACTOR * y0, AA_FACTOR * 40, AA_FACTOR * 40, 0, 360);  
  GUI_SetColor(GUI_WHITE);  
  GUI_AA_FillCircle(AA_FACTOR * x0, AA_FACTOR * y0, AA_FACTOR * 38); 
  GUI_SetBkColor(GUI_TRANSPARENT);

  GUI_SetBkColor(GUI_WHITE);
  GUI_SetColor(GUI_GRAY);  
  GUI_DispStringAt("12", x0 - 5, y0 - 38 + 5);
  GUI_DispStringAt( "6", x0 - 2, y0 + 38 - 13);
  GUI_DispStringAt( "9", x0 - 38 + 5, y0 - 5);
  GUI_DispStringAt( "3", x0 + 38 - 10, y0 - 3);
  
  for (i = 0; i <= 12; i++) 
  {
    a = i * 30000;
    SinHQ = GUI__SinHQ(a);
    CosHQ = GUI__CosHQ(a);

    xPos = x0 + ((36 * CosHQ) >> 16);
    yPos = y0 - ((36 * SinHQ) >> 16);
    
    GUI_AA_FillCircle(AA_FACTOR * xPos, AA_FACTOR * yPos, AA_FACTOR * 1); 
  }
  
  GUI_MagnifyPolygon(aPointsDest[0], aPoints[0], 4, 4);
  GUI_RotatePolygon(aPointsDest[0], aPointsDest[0], 4, - 2 * PI * (float)((float)hour + (float)min /60) / 12);    
  DrawNeedle(0, x0, y0);
  
  GUI_MagnifyPolygon(aPointsDest[1], aPoints[1], 4, 4);
  GUI_RotatePolygon(aPointsDest[1], aPointsDest[1], 4, - 2 * PI * (float)((float)min + (float)sec / 60) / 60);
  DrawNeedle(1, x0, y0);  
  
  GUI_MagnifyPolygon(aPointsDest[2], aPoints[2], 4, 1);
  GUI_RotatePolygon(aPointsDest[2], aPointsDest[2], 4, - 2 * PI * sec / 60);    
  DrawNeedle(2, x0, y0); 
  GUI_AA_DisableHiRes();
}




/**************************************************************
*函数功能：日历APP日期设置回调函数
*传入参数：
*返回值  ：
**************************************************************/
static void _cbDateSettings(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int Id, NCode;  
  static CALENDAR_DATE  hDate;
	
	RTC_DateTypeDef RTC_DateStructure;
	
  hItem = pMsg->hWin;
  switch (pMsg->MsgId) 
	{
    
		case WM_INIT_DIALOG:
			RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure); //获取日期
			CALENDAR_Create(pMsg->hWin,10, 20, 2000 + RTC_DateStructure.RTC_Year, RTC_DateStructure.RTC_Month, RTC_DateStructure.RTC_Date, 2, ID_CALENDAR, WM_CF_SHOW);//绘制日期模板
			CALENDAR_SetDefaultFont(CALENDAR_FI_CONTENT,GUI_FONT_13_1 );
			CALENDAR_SetDefaultFont(CALENDAR_FI_HEADER, GUI_FONT_13_1);
			break;
			
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);   
			NCode = pMsg->Data.v;          
			switch (NCode)				
			{
				case WM_NOTIFICATION_RELEASED: 
					switch (Id) 
					{
						case ID_BUTTON_APPLYDATESETTINGS:

							hItem = WM_GetDialogItem(pMsg->hWin, ID_CALENDAR);
							CALENDAR_GetSel (hItem, &hDate);
							
							if((hDate.Day > 0) && (hDate.Day <= 31) && 
								 (hDate.Month > 0)&& (hDate.Month <= 12) && 
									 (hDate.Year >= 2000))//设置日期
							{				
								RTC_DateStructure.RTC_WeekDay = 0;       
								RTC_DateStructure.RTC_Date = hDate.Day;;         
								RTC_DateStructure.RTC_Month = hDate.Month;;         
								RTC_DateStructure.RTC_Year = hDate.Year - 2000;;        
								RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStructure);
								RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							}
							break;  
					}
					break;
			}      
			break;    
		default:
			WM_DefaultProc(pMsg);
  }
}

/**************************************************************
*函数功能：日历APP时间设置回调函数
*传入参数：
*返回值  ：
**************************************************************/
static void _cbClockSettings(WM_MESSAGE * pMsg) 
{
  WM_HWIN hItem;
  int Id, NCode;  
  static WM_HTIMER hTimerTime;
  static uint8_t DisableAutoRefresh = 0;
   RTC_TimeTypeDef RTC_TimeStructure;
  hItem = pMsg->hWin;
  switch (pMsg->MsgId) 
	{
    
		case WM_TIMER:
			WM_InvalidateWindow(pMsg->hWin);
			WM_RestartTimer(pMsg->Data.v, 1000);
			break; 
			
		case WM_DELETE:
			WM_DeleteTimer(hTimerTime);
			DisableAutoRefresh = 0;
			break;
			
		case WM_INIT_DIALOG:
			
			hTimerTime = WM_CreateTimer(pMsg->hWin, 0, 1000, 0); 
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_HOUR);//创建时间设置复选按钮
			SPINBOX_SetRange(hItem, 0, 23);
				
			hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_MINUTE);
			SPINBOX_SetRange(hItem, 0, 59);
				
			hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_SEC);
			SPINBOX_SetRange(hItem, 0, 59);
				
			break;
			
		case WM_PAINT:
			if(DisableAutoRefresh == 0)
			{
				RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);   //备份时间
				GUI_UpdateClock (110,60,RTC_TimeStructure.RTC_Hours,RTC_TimeStructure.RTC_Minutes,RTC_TimeStructure.RTC_Seconds);
				
				hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_HOUR);
				SPINBOX_SetValue(hItem,RTC_TimeStructure.RTC_Hours);//设置时间
				
				hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_MINUTE);
				SPINBOX_SetValue(hItem,RTC_TimeStructure.RTC_Minutes);
				
				hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_SEC);
				SPINBOX_SetValue(hItem,RTC_TimeStructure.RTC_Seconds); 
			}
			else
			{
				hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_HOUR);
				RTC_TimeStructure.RTC_Hours = SPINBOX_GetValue(hItem);//获取时间
				
				hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_MINUTE);
				RTC_TimeStructure.RTC_Minutes = SPINBOX_GetValue(hItem); 
				
				hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_SEC);
				RTC_TimeStructure.RTC_Seconds = SPINBOX_GetValue(hItem); 
				
				GUI_UpdateClock (110, 60,RTC_TimeStructure.RTC_Hours,RTC_TimeStructure.RTC_Minutes,RTC_TimeStructure.RTC_Seconds);      
			}
			break;
			
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);   
			NCode = pMsg->Data.v;           
			switch (NCode) 
			{
				
				case WM_NOTIFICATION_CLICKED:      //点击时的操作设置
					switch (Id) 
					{
						case ID_SPINBOX_HOUR:
						case ID_SPINBOX_MINUTE:
						case ID_SPINBOX_SEC:
							if(DisableAutoRefresh == 0)
							{         
								DisableAutoRefresh = 1;
							} 
							break;        
					}
					break;
					
				case WM_NOTIFICATION_RELEASED: //释放时的操作设置
					switch (Id) 
					{
						case ID_BUTTON_APPLYCLKSETTINGS:
							hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_SEC);
							RTC_TimeStructure.RTC_Seconds = SPINBOX_GetValue(hItem);
							
							hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_MINUTE);
							RTC_TimeStructure.RTC_Minutes = SPINBOX_GetValue(hItem); 
							
							hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_HOUR);
							RTC_TimeStructure.RTC_Hours = SPINBOX_GetValue(hItem); 
							RTC_TimeStructure.RTC_H12 = RTC_H12_AMorPM;     
							RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure);
							RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);

							DisableAutoRefresh = 0;
							break;  
					}
					break;
			}      
			break;    
		default:
			WM_DefaultProc(pMsg);
  }
}

/**************************************************************
*函数功能：日历APP回调函数
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
			FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT, 0); //右上角关闭button     
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_INFO);
			IMAGE_SetBitmap(hItem, &bminfo);
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIPAGE);
		 
			hDialog = GUI_CreateDialogBox(_aDialogDateSettings, 
																		GUI_COUNTOF(_aDialogDateSettings), 
																		&_cbDateSettings, WM_UNATTACHED, 0, 0);
			MULTIPAGE_AddPage(hItem, hDialog, "Date Settings");
			
			hDialog = GUI_CreateDialogBox(_aDialogClockSettings, 
																		GUI_COUNTOF(_aDialogClockSettings), 
																		&_cbClockSettings, WM_UNATTACHED, 0, 0);
			MULTIPAGE_AddPage(hItem, hDialog, "Clock Settings");
			
			MULTIPAGE_SelectPage(hItem, 0);  
			
			break;
		 
		default:
			WM_DefaultProc(pMsg);
			break;
  }
}

/**************************************************************
*函数功能：日历APP
*传入参数：
*返回值  ：
**************************************************************/
void CreateCalender(WM_HWIN hWin)
{
  GUI_CreateDialogBox(_aDialog, GUI_COUNTOF(_aDialog), _cbDialog,hWin, 0, 26);
}



