#include "iconviewdemo.h"
#include "iconbitmap.h"
#include "background.h"
#include "DIALOG.h"
#include "FRAMEWIN.h"
#include "TestAPP.h"
#include "cpuusage.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "MESSAGEBOX.h"


#define ID_ICONVIEW_0   (GUI_ID_USER + 0x01)
#define ID_TIMER_TIME   (GUI_ID_USER + 0x02)
#define ID_SPRITE_TIME  (GUI_ID_USER + 0x03)
#define ID_BUTTON_BKGND (GUI_ID_USER + 0x04)
#define ID_MENU_LOG     (GUI_ID_USER + 0x05)
#define ID_MENU_PMGR    (GUI_ID_USER + 0x06)
#define ID_MENU_EXIT    (GUI_ID_USER + 0x07)
#define ID_MENU_PERF    (GUI_ID_USER + 0x08)

uint8_t strMonth[][12] = {"01","02","03","04","05","06","07","08","09","10","11","12"};

WM_HWIN IconviewWin;    //ICONVIEW控件窗口

//app图标描述结构体
typedef struct{
	const GUI_BITMAP 	*pBitmap;   //app图标
	const char			*pText;     //app图标名字
}BITMAP_ITEM;

//ICONVIEW显示的APP图标
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





static MENU_ITEM _aMenuItems[] = 
{
  {"Kernel Log"         , ID_MENU_LOG,  0},
  {"Process Viewer"    , ID_MENU_PMGR, 0},
  {"CPU Usage", ID_MENU_PERF, 0},  
  {0                    , 0           ,  MENU_IF_SEPARATOR},
  {"Cancel"             , ID_MENU_EXIT, 0},
};



/**
  * @brief  Adds one menu item to the given menu
  * @param  hMenu:    pointer to the handle of menu
  * @param  hSubmenu: pointer to the handle of Sub menu
  * @param  pText:    pointer to menu item description
  * @param  Id:       ID of the menu item
  * @param  Flags:    window creation flags
  * @retval None
  */
static void _AddMenuItem(MENU_Handle hMenu, MENU_Handle hSubmenu, const char* pText, U16 Id, U16 Flags) 
{
  MENU_ITEM_DATA Item;
  Item.pText    = pText;
  Item.hSubmenu = hSubmenu;
  Item.Flags    = Flags;
  Item.Id       = Id;
  MENU_AddItem(hMenu, &Item);
}


/**
  * @brief  The function opens a popup menu at the given position. It returns
  *         immediately after creation. On the first call it creates the menu.
  * @param  hParent:    pointer to the handle of the parent
  * @param  pMenuItems: pointer to menu items 
  * @param  NumItems:   number of menu items 
  * @param  x:          x position of the popup
  * @param  y:          y position of the popup 
  * @retval None
  */
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


//桌面窗口WM_HBKWIN回调函数
void cb_BkWindow(WM_MESSAGE *pMsg)
{
	int Id;
	int NCode;
	static int Iconview0_Sel;
	
	switch(pMsg->MsgId)
	{
		//用于绘制背景
		case WM_PAINT:
			GUI_SetBkColor(GUI_BLACK);
			GUI_Clear();                        
			GUI_DrawBitmap(&bmbackground,0,0);  //绘制背景图片
			break;
		
		case WM_NOTIFY_PARENT:
			Id=WM_GetId(pMsg->hWinSrc);
			NCode=pMsg->Data.v;
			switch(Id)
			{
				//消息为ICONVIEW0控件
				case ID_ICONVIEW_0:	
					switch(NCode)	//通知代码
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						//ICONVIEW0控件释放后
						case WM_NOTIFICATION_RELEASED:
							Iconview0_Sel=ICONVIEW_GetSel(pMsg->hWinSrc);
							switch(Iconview0_Sel)
							{
								case 0:		//APP0
                  if(BagAPPWin==NULL)CreateBagAPP();      //创建BagAPP
									break;
								case 1:		//APP1
                  if(BookAPPWin==NULL)CreateBookAPP();    //创建BookAPP
									break;
                case 2:		//APP2
                  if(BookshAPPWin==NULL)CreateBookshAPP();    //创建BookshAPP
									break;
                case 3:		//APP3
                  if(BrowerAPPWin==NULL)CreateBrowerAPP();    //创建BrowerAPP
									break;
							}
							break;
						case WM_NOTIFICATION_SEL_CHANGED:
							break;	
					}
					break;
				//消息为左上角的按键创建一个菜单
				case ID_BUTTON_BKGND:
					switch(NCode)
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						//按键释放后
						case WM_NOTIFICATION_RELEASED:					
							_OpenPopup(WM_HBKWIN, _aMenuItems, GUI_COUNTOF(_aMenuItems),0 , 25); 
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

/************************************************
*函数功能:状态栏回调函数
*传入参数：
*返回值  ：
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
    sprintf((char *)TempStr, "%d %%", minor);
    
//    sprintf((char *)TempStr, "%d %%", rt_get_CPUusage());
//    
//    if(rt_get_CPUusage() > 95 )
//    {
//      /* waiting for user input when using dialog box*/
//      sprintf((char *)TempStr, "95 %%");
//    }   
    GUI_DispStringAt( (char *)TempStr, 42, 4);   
    break;
    
  default:
    WM_DefaultProc(pMsg);
  }
}






/************************************************
*函数功能：按键重绘回调函数
*传入参数：
*返回值  ：
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
		GUI_DrawBitmap(&bmbuttonuser1, 0, 0);//未曾按下
  }
  else
  {
	  GUI_DrawBitmap(&bmbuttonuser2, 0, 0);//已经按下
  }
}

/************************************************
*函数功能：按键重绘回调函数
*传入参数：
*返回值  ：
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



//ICONVIEW演示例程
void iconviewdemo(void) 
{
	int i;
	WM_HWIN  hItem;
	//设置换肤 
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
	
	WM_SetCallback(WM_HBKWIN,cb_BkWindow); //设置桌面窗口WM_HBKWIN的回调函数
	//建立一个ICONVIEW作为主界面
	
	WM_CreateWindowAsChild(0,
                         0,
                         LCD_GetXSize(),
                         25,
                         WM_HBKWIN, 
                         WM_CF_SHOW | WM_CF_HASTRANS , 
                         _cbStatus, 
                         0);
	
	IconviewWin=ICONVIEW_CreateEx(0,25,                    //左上角坐标
							LCD_GetXSize(),LCD_GetYSize()- 26,                        //小工具的水平和垂直尺寸
							WM_HBKWIN,                      //父窗口为背景窗口
							WM_CF_SHOW|WM_CF_HASTRANS,      
							ICONVIEW_CF_AUTOSCROLLBAR_V,    //自动增加垂直滚动条
							ID_ICONVIEW_0,                  //小工具ID
							105,                            //图标的水平尺寸为100
							85);                           //图标的垂直尺寸为100
	ICONVIEW_SetFont(IconviewWin,&GUI_Font16_ASCII);
	ICONVIEW_SetBkColor(IconviewWin,ICONVIEW_CI_SEL,GUI_WHITE|0X70000000);
	ICONVIEW_SetSpace(IconviewWin,GUI_COORD_X,10);
	ICONVIEW_SetSpace(IconviewWin,GUI_COORD_Y,10);
	ICONVIEW_SetIconAlign(IconviewWin,ICONVIEW_IA_HCENTER|ICONVIEW_IA_VCENTER);
	for(i=0;i<(GUI_COUNTOF(BitmapItemTab1));i++)
	{
		ICONVIEW_AddBitmapItem(IconviewWin,BitmapItemTab1[i].pBitmap,BitmapItemTab1[i].pText);
	}
	hItem = BUTTON_CreateEx(0, 2, 31, 20, WM_HBKWIN, WM_CF_SHOW, 0, ID_BUTTON_BKGND);//创建按键
  WM_SetCallback(hItem, _cbButton);//关联回调函数
}
