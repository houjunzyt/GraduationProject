#include "lateclipping.h"
#include "GUI.h"
#include "WM.h"
#include "FRAMEWIN.h"
#include "BUTTON.h"

static WM_HWIN _hWin1;
static WM_HWIN _hWin2;
static WM_HWIN _hBut1;
static WM_HWIN _hBut2;
static int     _PaintCount1;
static int     _PaintCount2;

static GUI_COLOR _aColors[]={
GUI_BLUE, GUI_YELLOW, GUI_RED, GUI_LIGHTCYAN, GUI_DARKRED, GUI_DARKRED
};


//背景窗口回调函数
static void _cbBkWin(WM_MESSAGE *pMsg)
{
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
			GUI_SetBkColor(GUI_BLACK);
			GUI_Clear();
			GUI_SetColor(0x0060FF);
			GUI_DispStringAt("PaintCount (Early):",0,0);
			GUI_DispDecAt(_PaintCount1,120,0,5);//十进制显示：显示的数值。要写入的客户端窗口 X 位置、Y 位置 （单位：像素），显示的位数 （最大为 10）。
			GUI_SetColor(0x00FFC0);
			GUI_DispStringAt("PaintCount (Late):",0,12);
			GUI_DispDecAt(_PaintCount2,120,12,5);
			break;
		case WM_NOTIFY_PARENT://按键按下时会发送这个消息
			if(pMsg->Data.v == WM_NOTIFICATION_RELEASED)  //按钮被释放时
			{
				if(pMsg->hWinSrc == _hBut1)	//button1被释放
				{
					WM_InvalidateWindow(_hWin1); 	//窗口1失效
					WM_InvalidateWindow(_hWin2);	//窗口2失效		
				}else if(pMsg->hWinSrc == _hBut2)	//button2被释放
				{
					_PaintCount1 = 0;
					_PaintCount2 = 0;
					WM_InvalidateWindow(pMsg->hWin); //主窗口失效
				}
			}
			break;
			default:
				WM_DefaultProc(pMsg);	
	}
}


//顶层窗口回调函数
static void _cbTop(WM_MESSAGE *pMsg)
{
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
			GUI_SetBkColor(GUI_MAGENTA);
			GUI_Clear();
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}

//框架窗口1回调函数
static void _cbFrameWin1(WM_MESSAGE *pMsg)
{
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
			GUI_SetBkColor(_aColors[_PaintCount1 % 6]); //设置背景颜色
			GUI_Clear();
			GUI_SetColor(0x0060FF);//圆的颜色
			GUI_FillCircle(25,25,15);//绘制填充的圆
			//以客户端窗口的圆心的 X ,Y 位置 （像素），圆的半径 （直径的一半）。
			GUI_SetColor(GUI_BLACK);//圆的轮廓颜色
			GUI_DrawCircle(25,25,15);//绘制圆的轮廓
			_PaintCount1++;
			WM_InvalidateWindow(WM_HBKWIN); //背景窗口失效
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}

//框架窗口2回调函数
static void _cbFrameWin2(WM_MESSAGE *pMsg)
{
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
			GUI_SetBkColor(_aColors[_PaintCount2 % 6]);
			GUI_Clear();
			GUI_SetColor(0x00FFC0);
			GUI_FillCircle(25,25,15);
			GUI_SetColor(GUI_BLACK);
			GUI_DrawCircle(25,25,15);
			_PaintCount2++;
			WM_InvalidateWindow(WM_HBKWIN);
			break;
		default:
			WM_DefaultProc(pMsg);
	}		
}


//测试函数
void _ShowDemo(void)
{
	WM_HWIN hWin0;
	WM_HWIN	hWin1;
	WM_HWIN hWin2;
	WM_HWIN	hFrame1;
	WM_HWIN hFrame2;
	WM_HWIN	hClient1;
	WM_HWIN hClient2;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);	// 使能CRC时钟，用于emwin验证	
	GUI_Init();
	WM_MULTIBUF_Enable(1);	// 开启多缓冲
	
	WM_SetCallback(WM_HBKWIN,_cbBkWin);
		/*	x0 小工具最左侧的像素 （在父坐标中）。
		y0 小工具最顶端的像素 （在父坐标中）。
		xsize 小工具的水平尺寸 （以像素为单位）。
		ysize 小工具的垂直尺寸 （以像素为单位）。
		hParent 父窗口的句柄。如果为 0，则新的 FRAMEWIN 小工具将是桌面 （顶级窗口）的子项。
		WinFlags 窗口创建标记。为了使小工具立即可见，通常为 WM_CF_SHOW （有关可用参数值的列表，请参阅
							“窗口管理器 (WM)”（第 289 页）一章中的 WM_CreateWindow()）。
		ExFlags :
				0 无功能。
				FRAMEWIN_CF_MOVEABLE 将新框架窗口设置为可移动状态。更多详细信息，请参 阅 “FRAMEWIN_SetMoveable()”（第 437 
		Id 小工具的窗口 ID。
		pTitle 显示在标题栏中的标题。
		cb 指向用户回调例程的指针。此用户回调例程由客户端窗口的窗口回调例程调用。*/
	hFrame1 = FRAMEWIN_CreateEx(10,30,140,140,0,WM_CF_SHOW,FRAMEWIN_CF_MOVEABLE,0,"Early Clipping",_cbFrameWin1); //创建框架窗口1,可移动
	hFrame2 = FRAMEWIN_CreateEx(170,30,140,140,0,WM_CF_SHOW,FRAMEWIN_CF_MOVEABLE,0,"Late Clipping",_cbFrameWin2); //创建框架窗口2,可移动
	hClient1= WM_GetClientWindow(hFrame1); 	//返回框架1的客户端窗口句柄
	hClient2= WM_GetClientWindow(hFrame2);	//返回框架2的客户端窗口句柄
	_hWin1 = WM_CreateWindowAsChild(0,0,WM_GetWindowSizeX(hClient1),WM_GetWindowSizeY(hClient1),hClient1,WM_CF_SHOW,_cbFrameWin1,0);
	_hWin2 = WM_CreateWindowAsChild(0,0,WM_GetWindowSizeX(hClient2),WM_GetWindowSizeY(hClient2),hClient2,WM_CF_SHOW|WM_CF_LATE_CLIP,_cbFrameWin2,0);
	//int x0, int y0,int width, int height,WM_HWIN hWinParent,U8 Style,WM_CALLBACK* cbint NumExtraBytes
	//父窗口在窗口坐标中的左上 X 位置、 Y 位置。窗口尺寸x、y、父窗口句柄、窗口标识、回调指针、分配字节数
	_hBut1 = BUTTON_CreateEx(10,210,140,20,0,WM_CF_SHOW,0,1);	//创建按钮1
	_hBut2 = BUTTON_CreateEx(170,210,140,20,0,WM_CF_SHOW,0,2);	//创建按钮2
	//x0 按钮的最左像素 （在父坐标中）。y0 按钮的最上像素 （在父坐标中）。xsize 按钮的水平尺寸 （单位：像素）。ysize 按钮的垂直尺寸 （单位：像素）。Id 按钮按下时要返回的 ID。
	//Flags 窗口创建标记。为了使小工具立即可见，通常为 WM_CF_SHOW 
	hWin0 = FRAMEWIN_CreateEx(60,80,40,40,	0, 	WM_CF_SHOW|WM_CF_STAYONTOP,FRAMEWIN_CF_MOVEABLE,0,"Top 0",_cbTop);
	hWin1 = FRAMEWIN_CreateEx(220,80,40,40,	0,	WM_CF_SHOW|WM_CF_STAYONTOP,FRAMEWIN_CF_MOVEABLE,0,"Top 1",_cbTop);
	hWin2 = FRAMEWIN_CreateEx(140,170,40,40,0, 	WM_CF_SHOW|WM_CF_STAYONTOP,FRAMEWIN_CF_MOVEABLE,0,"Top 2",_cbTop);
	
	FRAMEWIN_SetResizeable(hWin0,1);  	//窗口设置为可缩放
	FRAMEWIN_SetResizeable(hWin1,1);  	//窗口设置为可缩放
	FRAMEWIN_SetResizeable(hWin2,1);    //窗口设置为可缩放
	// hObj 框架窗口的句柄State 框架窗口可缩放时为 1，否则为 0。
	BUTTON_SetText(_hBut1,"Invalidate");	//设置按钮的名字
	BUTTON_SetText(_hBut2,"Reset counts");	
	//hObj 按钮的句柄。s 要显示的文本。
	while(1)
	{
		GUI_Delay(50);
	}
}

