#include "iconviewdemo.h"

static GRAPH_DATA_Handle hData = 0;
static GRAPH_SCALE_Handle hScale = 0;
static WM_HWIN  hPerformance = 0;
static WM_HWIN  hProcess = 0;
static WM_HWIN  hLog = 0;

//�˵��������
static const GUI_WIDGET_CREATE_INFO _aPerformanceDialogCreate[] = 
{
  { FRAMEWIN_CreateIndirect, "CPU Usage", ID_FRAMEWIN_PERFORMANCE, 10, 30, 220, 180, 0, 0x0, 0 },
  { GRAPH_CreateIndirect, "", ID_GRAPH_CPU, 5, 9, 200, 130, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Hide", ID_BUTTON_HIDE, 70, 142, 80, 30, 0, 0x0, 0 },
};

static const GUI_WIDGET_CREATE_INFO _aKernelLogDialogCreate[] = 
{
  { FRAMEWIN_CreateIndirect, "Kernel Log", ID_FRAMEWIN_KERNELLOG, 10, 30, 220, 180, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Cancel", ID_BUTTON_CANCEL_KERNELLOG, 70, 142, 80, 30, 0, 0x0, 0 },
  { MULTIEDIT_CreateIndirect, "Multiedit", ID_MULTIEDIT_KERNELLOG, 5, 9, 200, 130, 0, 0x0, 0 },
};

static const GUI_WIDGET_CREATE_INFO _aProcessManagerDialogCreate[] = 
{
  { FRAMEWIN_CreateIndirect, "Process viewer", ID_FRAMEWIN_PROCESSMANAGER, 10, 30, 220, 180, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Cancel", ID_BUTTON_CANCEL_PROCESSMANAGER, 70, 142, 80, 30, 0, 0x0, 0 },
  { LISTVIEW_CreateIndirect, "Listview", ID_LISTVIEW_PROCESSMANAGER, 5, 5, 200, 130, 0, 0x0, 0 },
};


uint8_t strMonth[][12] = {"01","02","03","04","05","06","07","08","09","10","11","12"};

WM_HWIN IconviewWin;    //ICONVIEW�ؼ�����

//appͼ�������ṹ��
typedef struct{
	const GUI_BITMAP 	*pBitmap;   //appͼ��
	const char			*pText;     //appͼ������
}BITMAP_ITEM;

//ICONVIEW��ʾ��APPͼ��
static const BITMAP_ITEM BitmapItemTab1[]={
	{&bmbag,		"Bag"},
	{&bmbook,		"Book"},
	{&bmbookshelf,	"Booksh"	},
	{&bmbrowser,	"Brower"},
	{&bmcalendar,	"Calendar"},
	{&bmclipboard,	"Clipboard"},
//	{&bmcompass,	"Compass"},
//	{&bmcompose,	"Cmpose"},
//	{&bmdocument,	"Cument"},
//	{&bmfolder,		"Folder"},
//	{&bmmail,		"Mail"},
//	{&bmpaint,		"Paint"},
//	{&bmpin,		"Pin"},
//	{&bmsettings,	"Set"},
//	{&bmtools,		"Tool"},
//	{&bmturntable,	"TurnTab"},
//	{&bmvideo,		"Video"},
//	{&bmwallet,		"Wallet"},
//	{&bmlightbulb,	"Light"},
//	{&bmstar,		"Star"},
};




/********************************************************
*�������ܣ��˵�����
*���������
*����ֵ  ��
********************************************************/
static MENU_ITEM _aMenuItems[] = 
{
  {"Kernel Log"         , ID_MENU_LOG,  0},//�ں�log
  {"Process Viewer"    , ID_MENU_PMGR, 0},//���񴰿�
  {"CPU Usage", ID_MENU_PERF, 0},  //cpuʹ����
  {0          , 0           ,  MENU_IF_SEPARATOR},//�ֽ���
  {"Cancel"             , ID_MENU_EXIT, 0},//ȡ��
};

/********************************************************
*�������ܣ���Ӳ˵���Ŀ
*���������
*����ֵ  ��
********************************************************/
static void _AddMenuItem(MENU_Handle hMenu, MENU_Handle hSubmenu, const char* pText, U16 Id, U16 Flags) 
{
  MENU_ITEM_DATA Item;
  Item.pText    = pText;
  Item.hSubmenu = hSubmenu;
  Item.Flags    = Flags;
  Item.Id       = Id;
  MENU_AddItem(hMenu, &Item);
}

/********************************************************
*�������ܣ������˵�
*���������
*����ֵ  ��
********************************************************/
static void _OpenPopup(WM_HWIN hParent, MENU_ITEM * pMenuItems, int NumItems, int x, int y) 
{
  static MENU_Handle hMenu;
  if (!hMenu) {
    int i;
    /* Create the popup window only one time */
    hMenu = MENU_CreateEx(0, 0, 0, 0, WM_UNATTACHED, 0, MENU_CF_VERTICAL, 0);
    MENU_SetBkColor(hMenu, MENU_CI_SELECTED, GUI_LIGHTBLUE);
    
    for (i = 0; i < NumItems; i++) {
      _AddMenuItem(hMenu, 0, pMenuItems[i].sText, pMenuItems[i].Id, pMenuItems[i].Flags);
    }
  }

  MENU_Popup(hMenu, hParent, x, y, 0, 0, 0);
}

/********************************************************
*�������ܣ���ʱ����CPUʹ������
*���������
*����ֵ  ��
********************************************************/
static void _cbCpuWindow(WM_MESSAGE * pMsg) {
  uint8_t major,minor;
  static WM_HTIMER hTimerTime; 
  switch (pMsg->MsgId) 
  {
		case WM_CREATE:
			hTimerTime = WM_CreateTimer(pMsg->hWin, 0, 400, 0); //����һ����ʱ��       
			break;
			
		case WM_TIMER:
			cpu_usage_get(&major,&minor);//��ȡcpuʹ����
			GRAPH_DATA_YT_AddValue(hData , major);
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
*�������ܣ��˵���CPUʹ�����������Ҫ����һ����ܽ��� ������ʾ������Ļص�����_cbCpuWindow��
*���������
*����ֵ  ��
********************************************************/
static void _cbPerformanceDialog(WM_MESSAGE * pMsg) {
  int NCode;
  int Id;
  WM_HWIN hGraph;

  GUI_RECT Rect;

  switch (pMsg->MsgId) 
  {
		case WM_INIT_DIALOG:

			FRAMEWIN_SetTitleVis(pMsg->hWin, 0);
			hGraph = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_CPU);
			hData = GRAPH_DATA_YT_Create(GUI_LIGHTGREEN, 500, 0, 20);
			GRAPH_SetGridVis(hGraph, 1);
			GRAPH_SetBorder(hGraph, 30, 2, 2, 2); 
			GRAPH_AttachData(hGraph, hData);
			hScale = GRAPH_SCALE_Create(20, GUI_TA_RIGHT, GRAPH_SCALE_CF_VERTICAL, 25);
			GRAPH_AttachScale(hGraph, hScale);  
			GRAPH_SCALE_SetTextColor(hScale, GUI_YELLOW);
			GRAPH_SetGridDistX(hGraph, 25);
			GRAPH_SetGridDistY(hGraph, 25);
			WM_GetClientRect(&Rect);
			WM_CreateWindowAsChild(230, 0, 10, 10, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbCpuWindow , 0); 
			
			break;
			
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id) {
				
			case ID_BUTTON_HIDE: /* Notifications sent by Hide Button */
				switch(NCode) 
				{
				case WM_NOTIFICATION_RELEASED:
					
					if(hPerformance != 0)
					{
						WM_HideWindow(hPerformance);
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
*�������ܣ����������������Ϣ��ʱˢ��
*���������
*����ֵ  ��
********************************************************/
static void _cbTaskWindow(WM_MESSAGE * pMsg) {
  
  static WM_HTIMER hTimerTime; 

  switch (pMsg->MsgId) 
  {
  case WM_CREATE:
    /* Create timer */
    hTimerTime = WM_CreateTimer(pMsg->hWin, 0, 1000, 0);        
    break;
    
  case WM_TIMER:
    WM_InvalidateWindow(WM_GetParent(pMsg->hWin));
    WM_RestartTimer(pMsg->Data.v, 1000);
    break; 
    
  case WM_DELETE:
    WM_DeleteTimer(hTimerTime);
    break;
    
  default:
    WM_DefaultProc(pMsg);
  }
}

/********************************************************
*�������ܣ������������ȡ�̵߳�״̬
*���������
*����ֵ  ��
********************************************************/
static void _UpdateProcessManagerView(WM_HWIN  hItem) {
  int      Idx;
  char     str[3];
  RT_Thread_Info thread_info[LIST_FIND_OBJ_NR]={0};
	list_thread(thread_info );//��ȡ�߳���Ϣ
  for (Idx = 0; Idx < thread_info[0].num; Idx ++)
  {
    LISTVIEW_SetItemText(hItem, 0, Idx, (char *)(thread_info[Idx].name)); 
    sprintf(str, "%lu", (unsigned long)thread_info[Idx].current_priority);
    LISTVIEW_SetItemText(hItem, 1, Idx, str);
    
    switch (thread_info[Idx].stat)
    {
			case RT_THREAD_READY:
				LISTVIEW_SetItemText(hItem, 2, Idx, "Ready"); 
				break;
				
			case RT_THREAD_SUSPEND:
				LISTVIEW_SetItemText(hItem, 2, Idx, "Suspended"); 
				break;
				
			case RT_THREAD_INIT:
				LISTVIEW_SetItemText(hItem, 2, Idx, "Init"); 
				break;
				
			case RT_THREAD_CLOSE:
				LISTVIEW_SetItemText(hItem, 2, Idx, "Close"); 
				break;
				
			case RT_THREAD_RUNNING:
				LISTVIEW_SetItemText(hItem, 2, Idx, "Running"); 
				break;        
			default:
				LISTVIEW_SetItemText(hItem, 2, Idx, "Unknown"); 
				break;        
    }
  }
  LISTVIEW_SetSort(hItem, 1, 0);
}

/********************************************************
*�������ܣ��������������ص�����
*���������
*����ֵ  ��
********************************************************/
static void _cbProcessManagerDialog(WM_MESSAGE * pMsg) 
{
  WM_HWIN  hItem;
  int      NCode;
  int      Id, Idx;
  switch (pMsg->MsgId) 
	{
    
		case WM_INIT_DIALOG:
			hItem = pMsg->hWin;
			FRAMEWIN_SetTitleVis(hItem, 0);
			hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_PROCESSMANAGER);
			LISTVIEW_AddColumn(hItem, 90, "Threads", GUI_TA_HCENTER | GUI_TA_VCENTER);//�����Ŀ
			LISTVIEW_AddColumn(hItem, 50, "Priority", GUI_TA_HCENTER | GUI_TA_VCENTER);
			LISTVIEW_AddColumn(hItem, 60, "State", GUI_TA_HCENTER | GUI_TA_VCENTER);
			
			LISTVIEW_SetGridVis(hItem, 1);
			LISTVIEW_SetTextAlign(hItem, 0, GUI_TA_LEFT);
			LISTVIEW_SetTextAlign(hItem, 1, GUI_TA_HCENTER);
			LISTVIEW_SetTextAlign(hItem, 2, GUI_TA_HCENTER);
			HEADER_SetDragLimit(LISTVIEW_GetHeader(hItem), 1);
			LISTVIEW_SetCompareFunc(hItem, 1, LISTVIEW_CompareDec);
			LISTVIEW_SetTextColor(hItem, LISTVIEW_CI_SELFOCUS, GUI_LIGHTBLUE);  

			for (Idx = 0; Idx < 16; Idx ++)
			{
				LISTVIEW_AddRow(hItem, NULL);
			}

			_UpdateProcessManagerView(hItem);
			
			WM_CreateWindowAsChild(230, 0, 10, 10, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbTaskWindow , 0); 
			break;

		case WM_PAINT:
			hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_PROCESSMANAGER);
			_UpdateProcessManagerView(hItem);
			break;

		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id) 
			{
				case ID_BUTTON_CANCEL_PROCESSMANAGER: /* Notifications sent by 'Cancel' button */
				switch(NCode) 
				{
					case WM_NOTIFICATION_RELEASED:
						GUI_EndDialog(pMsg->hWin, 0);
						hProcess = 0;       
						break;
				}
				break;
			case ID_LISTVIEW_PROCESSMANAGER: /* Notifications sent by 'Listview' */
				switch(NCode) 
				{
					case WM_NOTIFICATION_CLICKED:
						break;
					case WM_NOTIFICATION_RELEASED:
						break;
					case WM_NOTIFICATION_SEL_CHANGED:
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

void k_UpdateLog(char *Msg)
{
  WM_HWIN hItem;
  
  if(hLog != 0)
  {
    hItem = WM_GetDialogItem(hLog, ID_MULTIEDIT_KERNELLOG);
    MULTIEDIT_SetText(hItem, Msg);
    MULTIEDIT_SetCursorOffset(hItem, LOG_IN_ptr);    
  }
}

/**
  * @brief  Callback function of the kernel log dialog
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbKernelLogDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    
    hItem = pMsg->hWin;
    FRAMEWIN_SetTitleVis(hItem, 0);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_KERNELLOG);
    MULTIEDIT_SetFont(hItem, GUI_FONT_8_1);
    MULTIEDIT_SetTextColor(hItem, MULTIEDIT_CI_READONLY, GUI_GREEN); 
    MULTIEDIT_SetBkColor(hItem, MULTIEDIT_CI_READONLY, GUI_BLACK); 
    MULTIEDIT_SetReadOnly(hItem, 1);
    MULTIEDIT_SetText(hItem, (char *)pLOG_CacheBuffer);
    MULTIEDIT_SetCursorOffset(hItem, LOG_IN_ptr);
    MULTIEDIT_SetAutoScrollV(hItem, 1);
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_CANCEL_KERNELLOG: /* Notifications sent by 'Cancel' button */
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        GUI_EndDialog(pMsg->hWin, 0); 
        hLog = 0;
        break;
      }
      break;
    case ID_MULTIEDIT_KERNELLOG: /* Notifications sent by 'Multiedit' */
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
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
*�������ܣ��������ڻص�����
*���������
*����ֵ  ��
********************************************************/
void cb_BkWindow(WM_MESSAGE *pMsg)
{
	int Id;
	int NCode;
	static int Iconview0_Sel;
	MENU_MSG_DATA* pData;
	
	switch(pMsg->MsgId)
	{
		//���ڻ��Ʊ���
		case WM_PAINT:
			GUI_SetBkColor(GUI_BLACK);
			GUI_Clear();                        
			GUI_DrawBitmap(&bmbackground,0,0);  //���Ʊ���ͼƬ
			break;
		
		case WM_NOTIFY_PARENT:
			Id=WM_GetId(pMsg->hWinSrc);
			NCode=pMsg->Data.v;
			switch(Id)
			{
				//��ϢΪICONVIEW0�ؼ�
				case ID_ICONVIEW_0:	
					switch(NCode)	//֪ͨ����
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						//ICONVIEW0�ؼ��ͷź�
						case WM_NOTIFICATION_RELEASED:
							Iconview0_Sel=ICONVIEW_GetSel(pMsg->hWinSrc);
							switch(Iconview0_Sel)
							{
								case 0:		//APP0
                  if(BagAPPWin==NULL)CreateBagAPP();      //����BagAPP
									break;
								case 1:		//APP1
                  if(BookAPPWin==NULL)CreateBookAPP();    //����BookAPP
									break;
                case 2:		//APP2
                  if(BookshAPPWin==NULL)CreateBookshAPP();    //����BookshAPP
									break;
                case 3:		//APP3
                  if(BrowerAPPWin==NULL)CreateBrowerAPP();    //����BrowerAPP
									break;
								case 4:
//									k_UpdateLog("touch app5\n");
									break;
								case 5:
//									k_UpdateLog("touch app6\n");
									break;
									
							}
							break;
						case WM_NOTIFICATION_SEL_CHANGED:
							break;	
					}
					break;
				//��ϢΪ���Ͻǵİ�������һ���˵�
				case ID_BUTTON_BKGND:
					switch(NCode)
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						//�����ͷź�
						case WM_NOTIFICATION_RELEASED:					
							_OpenPopup(WM_HBKWIN, _aMenuItems, GUI_COUNTOF(_aMenuItems),0 , 25); 
							break;
						case WM_NOTIFICATION_SEL_CHANGED:
							break;				
					}
					break;	
			}
			break;
		//�˵������
		case WM_MENU:
			pData = (MENU_MSG_DATA*)pMsg->Data.p;
			switch (pData->MsgType) 
			{
				//�˵��ڵĶ����Ѿ���ѡ��,����������ź�
				case MENU_ON_ITEMSELECT:
					switch(pData->ItemId)//�������Ŀ�ź�
					{
						case ID_MENU_LOG:
							rt_kprintf("kernel log\n");
							if (hLog == 0)
							{
							hLog = GUI_CreateDialogBox(_aKernelLogDialogCreate, 
																				 GUI_COUNTOF(_aProcessManagerDialogCreate), 
																				 _cbKernelLogDialog, 
																				 pMsg->hWin, 
																				 0, 
																				 0);  
							}
							else
							{
							WM_ShowWindow(hLog);
							WM_BringToTop(hLog);
							}        
							break;
						case ID_MENU_PMGR:
							if(hProcess == 0)
							{
								hProcess = GUI_CreateDialogBox(_aProcessManagerDialogCreate, 
																							 GUI_COUNTOF(_aProcessManagerDialogCreate), 
																							 _cbProcessManagerDialog, 
																							 pMsg->hWin, 
																							 0, 
																							 0);  
							}
							else
							{
								WM_ShowWindow(hProcess);
								WM_BringToTop(hProcess);
							}
							break;
						case ID_MENU_PERF:
						  if(hPerformance == 0)//����һ��ͼ��������
							{
								hPerformance = GUI_CreateDialogBox(_aPerformanceDialogCreate, 
																									 GUI_COUNTOF(_aPerformanceDialogCreate), 
																									 _cbPerformanceDialog, //����
																									 pMsg->hWin, 
																									 0, 
																									 0);
							}
							else
							{
								WM_ShowWindow(hPerformance);
								WM_BringToTop(hPerformance);
							}
							break;
					  case ID_MENU_EXIT:
							rt_kprintf("cancel\n");
							break;
					}
			}
			break;
		default:
			WM_DefaultProc(pMsg);
			break;		
	}
	
}

/************************************************
*��������:״̬���ص�����
*���������
*����ֵ  ��
************************************************/
static void _cbStatus(WM_MESSAGE * pMsg) 
{
  int xSize, ySize;
  static uint8_t TempStr[50];
  static WM_HTIMER hTimerTime; 
	rt_uint8_t major,minor;
  uint8_t sec, min, hour, day, month;
  uint16_t year;
  WM_HWIN hWin;

  hWin = pMsg->hWin;
  switch (pMsg->MsgId) 
  {
  case WM_CREATE:
    hTimerTime = WM_CreateTimer(hWin, ID_TIMER_TIME, 1000, 0);
//    if(settings.b.enable_sprite)
//    {
//      hSpriteTime = WM_CreateTimer(hWin, ID_SPRITE_TIME, 50, 0);    
//    }
    break;
    
  case WM_DELETE:
    WM_DeleteTimer(hTimerTime);
//    if(settings.b.enable_sprite)
//    {    
//      WM_DeleteTimer(hSpriteTime);    
//    }
    break;
    
  case WM_TIMER:
    if( WM_GetTimerId(pMsg->Data.v) == ID_TIMER_TIME)
    {
      WM_InvalidateWindow(hWin);
      WM_RestartTimer(pMsg->Data.v, 1000);
    }
    else if( WM_GetTimerId(pMsg->Data.v) == ID_SPRITE_TIME)
    {
//      if(settings.b.enable_sprite)
//      {    
//        if(SpriteDisabled == 0)
//        {
//          _MoveSprite(&_aSprite[0]);
//        }
//      }
      WM_RestartTimer(pMsg->Data.v, 100);
    }
    break;
    
  case WM_PAINT:
    xSize = WM_GetWindowSizeX(hWin);
    ySize = WM_GetWindowSizeY(hWin);
    
    /* Draw background */
    GUI_SetColor(0x40303030);
    GUI_FillRect(0, 0, xSize , ySize - 3);
    GUI_SetColor(0x808080);
    GUI_DrawHLine(ySize - 2, 0, xSize );
    GUI_SetColor(0x404040);
    GUI_DrawHLine(ySize - 1, 0, xSize );

    /* Draw time & Date */
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(GUI_FONT_13B_ASCII);
    
//    k_GetTime(&RTC_Time);
    sec    =  16;
    min    =  16;
    hour   =  16;
    
//    k_GetDate(&RTC_DateStructure);
    
    sprintf((char *)TempStr,"%02d:%02d:%02d",hour, min, sec);
    GUI_DispStringAt((char *)TempStr, xSize - 50, 4);
    
    year =3 + 2015;
    month =2;
    day =1;
    
    if((day > 0) && (day <= 31) && 
       (month > 0)&& (month <= 12) && 
         (year >= 1900))
    {
      sprintf((char *)TempStr, "%04d/%s/%02d",year,strMonth[month-1],day);
    }
    else
    {
      sprintf((char *)TempStr, "2015/01/01");
    }

    GUI_DispStringHCenterAt((char *)TempStr, xSize / 2, 4);
    
    /* USB */
//    if(k_StorageGetStatus(USB_DISK_UNIT))
//    {
//       GUI_DrawBitmap(&bmusbdisk, xSize - 80, 0);
//    }
    cpu_usage_get(&major,&minor);
    sprintf((char *)TempStr, "%d.%d%%", major,minor); 
    GUI_DispStringAt( (char *)TempStr,33, 4);   
    break;
    
  default:
    WM_DefaultProc(pMsg);
  }
}

/************************************************
*�������ܣ������ػ�ص�����
*���������
*����ֵ  ��
************************************************/
static void _OnPaint(BUTTON_Handle hObj) 
{
  int Index;

  GUI_RECT Rect;
  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();  
  
  WM_GetClientRect(&Rect);
  
  /* Draw button background */
  if(Index)
  {
		GUI_DrawBitmap(&bmbuttonuser1, 0, 0);//δ������
  }
  else
  {
	  GUI_DrawBitmap(&bmbuttonuser2, 0, 0);//�Ѿ�����
  }
}

/************************************************
*�������ܣ������ػ�ص�����
*���������
*����ֵ  ��
************************************************/
static void _cbButton(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg); 
      break;
  }
}


/************************************************
*�������ܣ���������ͼ��
*���������
*����ֵ  ��
************************************************/
void iconviewdemo(void) 
{
	int i;
	WM_HWIN  hItem;
	//���û��� 
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	MULTIPAGE_SetDefaultSkin(MULTIPAGE_SKIN_FLEX);
	
	KernelLogInit();//��ʼ���ں˻�����
	
	WM_SetCallback(WM_HBKWIN,cb_BkWindow); //�������洰��WM_HBKWIN�Ļص�����
	//����һ��ICONVIEW��Ϊ������
	
	WM_CreateWindowAsChild(0,
                         0,
                         LCD_GetXSize(),
                         25,
                         WM_HBKWIN, 
                         WM_CF_SHOW | WM_CF_HASTRANS , 
                         _cbStatus, 
                         0);
	
	IconviewWin=ICONVIEW_CreateEx(0,25,                    //���Ͻ�����
							LCD_GetXSize(),LCD_GetYSize()- 26,                        //С���ߵ�ˮƽ�ʹ�ֱ�ߴ�
							WM_HBKWIN,                      //������Ϊ��������
							WM_CF_SHOW|WM_CF_HASTRANS,      
							ICONVIEW_CF_AUTOSCROLLBAR_V,    //�Զ����Ӵ�ֱ������
							ID_ICONVIEW_0,                  //С����ID
							105,                            //ͼ���ˮƽ�ߴ�Ϊ100
							85);                           //ͼ��Ĵ�ֱ�ߴ�Ϊ100
	ICONVIEW_SetFont(IconviewWin,&GUI_Font16_ASCII);
	ICONVIEW_SetBkColor(IconviewWin,ICONVIEW_CI_SEL,GUI_WHITE|0X70000000);
	ICONVIEW_SetSpace(IconviewWin,GUI_COORD_X,10);
	ICONVIEW_SetSpace(IconviewWin,GUI_COORD_Y,10);
	ICONVIEW_SetIconAlign(IconviewWin,ICONVIEW_IA_HCENTER|ICONVIEW_IA_VCENTER);
	for(i=0;i<(GUI_COUNTOF(BitmapItemTab1));i++)
	{
		ICONVIEW_AddBitmapItem(IconviewWin,BitmapItemTab1[i].pBitmap,BitmapItemTab1[i].pText);
	}
	hItem = BUTTON_CreateEx(0, 2, 31, 20, WM_HBKWIN, WM_CF_SHOW, 0, ID_BUTTON_BKGND);//��������
  WM_SetCallback(hItem, _cbButton);//�����ص�����
}
