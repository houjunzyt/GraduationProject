#include "sample.h"
#include "string.h"
#include "GUI.h"
#include "WM.h"	
#include "lcd_user.h"	
#include "rtthread.h"
#include <stdio.h>

#define MSG_CHANGE_TEXT (WM_USER + 0)  //用户自定义的消息类型

#define SPEED           1250

static char _acInfoText[40];

static GUI_COLOR _WindowColor1 = GUI_GREEN;	//窗口1的颜色为绿色
static GUI_COLOR _FrameColor1  = GUI_BLUE;	//窗口1的边界为蓝色
static GUI_COLOR _WindowColor2 = GUI_RED;	//窗口2的颜色为红色
static GUI_COLOR _FrameColor2  = GUI_YELLOW;//窗口2的边界为黄色
static GUI_COLOR _ChildColor   = GUI_YELLOW;//子窗口的颜色为黄色
static GUI_COLOR _ChildFrame   = GUI_BLACK;	//子窗口的边界为黑色

//回调
static WM_CALLBACK * _cbBkWindowOld;


//窗口句柄
static WM_HWIN _hWindow1;  	//窗口1
static WM_HWIN _hWindow2;	//窗口2
static WM_HWIN _hChild;		//子窗口


//向背景窗口发送一个消息并使背景窗口无效，
//背景窗口的回调函数就会显示发送的消息
static void _ChangeInfoText(char *pStr)
{
	WM_MESSAGE Message;
	Message.MsgId = MSG_CHANGE_TEXT;//用户消息
	Message.Data.p = pStr;
	WM_SendMessage(WM_HBKWIN,&Message); 
	//向背景窗口发送消息 ，消息类型以及要显示的字符串；hWin 窗口句柄；pMsg 消息指针
	WM_InvalidateWindow(WM_HBKWIN);	//会发送窗口重绘msg，在对应设置窗口句柄的回调函数里面调用
}

//显示一些信息
static void _DrawInfoText(char *pStr)
{
	GUI_SetColor(GUI_WHITE);
	GUI_SetFont(&GUI_Font24_ASCII);
	GUI_DispStringHCenterAt("WindowManager",LCD_X_LEN/2,5);
	//在指定位置水平居中显示字符串const char GUI_FAR * s, int x, int y
	//x:居中中心点 X坐标  y:要写入的客户端窗口与顶端的距离，单位像素
	GUI_SetFont(&GUI_Font8x16);
	GUI_DispStringAtCEOL(pStr,5,40); 
	//在指定位置显示字符串并清除至行末const char GUI_FAR * s, int x, int y
	//x,y显示的文本左上角坐标值
}


//窗口向上移动
//dy:需要移动坐标距离(像素数)
static void _LiftUp(int dy)
{
	int i;
	int tm;
	for(i=0;i<(dy/4);i++)
	{
		tm=GUI_GetTime();
		WM_MoveWindow(_hWindow1,0,-4); //移动窗口1
		WM_MoveWindow(_hWindow2,0,-4); //移动窗口2
		while((GUI_GetTime()-tm)<20)
		{
			WM_Exec(); //通过执行回调函数重绘无效窗口
		}
	}

}

//窗口向下移动
//dy:需要移动坐标距离(像素数)
static void _LiftDown(int dy)
{
	int i;
	int tm;
	for(i=0;i<(dy/4);i++)
	{
		tm= GUI_GetTime();
		WM_MoveWindow(_hWindow1,0,4);
		WM_MoveWindow(_hWindow2,0,4);
		while((GUI_GetTime()-tm)<20)
		{
			WM_Exec();
		}
	}
}


//背景窗口的回调函数
static void sample_cbBkWindow(WM_MESSAGE *pMsg)
{
	switch(pMsg->MsgId)
	{
		case MSG_CHANGE_TEXT://用户定义的消息，将要显示的数据写入消息，然后传递过来
			strcpy(_acInfoText,(char const *)pMsg->Data.p);
		case WM_PAINT:
			GUI_SetBkColor(GUI_BLACK);//背景颜色为黑色
			GUI_Clear();
			GUI_SetColor(GUI_WHITE);
			GUI_SetFont(&GUI_Font24_ASCII);
			GUI_DispStringHCenterAt("WindowManager",LCD_X_LEN/2,5);
			GUI_SetFont(&GUI_Font8x16);
			GUI_DispStringAt(_acInfoText,5,40);//显示的东西通过消息获取
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}

//窗口1回调函数
static void sample_cbWindow1(WM_MESSAGE *pMsg)
{
	GUI_RECT Rect;
	int x;
	int y;
	
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
			WM_GetInsideRect(&Rect); //返回客户区的坐标
			GUI_SetBkColor(_WindowColor1);
			GUI_SetColor(_FrameColor1);
			GUI_ClearRectEx(&Rect);   	//填充一个矩形区域
			GUI_DrawRectEx(&Rect);		//绘制一个矩形
			GUI_SetColor(GUI_WHITE);
			GUI_SetFont(&GUI_Font24_ASCII);
			x=WM_GetWindowSizeX(pMsg->hWin); 	//获取窗口的X轴大小
			y=WM_GetWindowSizeY(pMsg->hWin);	//获取窗口的Y轴大小	
			GUI_DispStringHCenterAt("Window 1",x/2,(y/2)-12);
			break;
		default:
			WM_DefaultProc(pMsg);
		
	}
}


//窗口2回调函数
static void _cbWindow2(WM_MESSAGE *pMsg)
{
	GUI_RECT Rect;
	int x;
	int y;
	
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
			WM_GetInsideRect(&Rect);	//返回客户区的坐标
			GUI_SetBkColor(_WindowColor2);
			GUI_SetColor(_FrameColor2);
			GUI_ClearRectEx(&Rect);		//填充一个矩形区域
			GUI_DrawRectEx(&Rect);		//绘制一个矩形
			GUI_SetColor(GUI_WHITE);
			GUI_SetFont(&GUI_Font24_ASCII);
			x = WM_GetWindowSizeX(pMsg->hWin);	//获取窗口的X轴大小
			y = WM_GetWindowSizeY(pMsg->hWin);	//获取窗口的Y轴大小
			GUI_DispStringHCenterAt("Window 2", x / 2, (y / 4) - 12);
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}
	
//子窗口的回调函数
static void _cbChild(WM_MESSAGE *pMsg)
{
	GUI_RECT Rect;
	int x;
	int y;
	
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
			WM_GetInsideRect(&Rect);
			GUI_SetBkColor(_ChildColor);//背景颜色黄色
			GUI_SetColor(_ChildFrame);//黑色边框
			GUI_ClearRectEx(&Rect);//填充一个矩形区域
			GUI_DrawRectEx(&Rect);	//绘制一个矩形
			GUI_SetColor(GUI_RED);
			GUI_SetFont(&GUI_Font24_ASCII);
			x = WM_GetWindowSizeX(pMsg->hWin);
			y = WM_GetWindowSizeY(pMsg->hWin);
			GUI_DispStringHCenterAt("Child window", x / 2, (y / 2) - 12);
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}
	
//窗口1的另一个回调函数
static void _cbDemoCallback1(WM_MESSAGE *pMsg)
{
	int x,y;
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
			GUI_SetBkColor(GUI_GREEN);
			GUI_Clear();
			GUI_SetColor(GUI_RED);
			GUI_SetFont(&GUI_FontComic18B_1);
			x=WM_GetWindowSizeX(pMsg->hWin);
			y = WM_GetWindowSizeY(pMsg->hWin);
			GUI_DispStringHCenterAt("Window 1\nanother Callback", x / 2, (y / 2) - 18);
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}


//窗口2的另一个回调函数
static void _cbDemoCallback2(WM_MESSAGE *pMsg)
{
	int x,y;
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
			GUI_SetBkColor(GUI_MAGENTA);
			GUI_Clear();
			GUI_SetColor(GUI_YELLOW);
			GUI_SetFont(&GUI_FontComic18B_1);
			x = WM_GetWindowSizeX(pMsg->hWin);
			y = WM_GetWindowSizeY(pMsg->hWin);
			GUI_DispStringHCenterAt("Window 2\nanother Callback", x / 2, (y / 4) - 18);
			break;	
		default:
			WM_DefaultProc(pMsg);
	}
}


//设置窗口的背景颜色,用来演示WM_SetDesktopColor()函数
static void _DemoSetDesktopColor(void)
{
	GUI_SetBkColor(GUI_BLUE);//设置背景颜色
	GUI_Clear();
	_DrawInfoText("WM_SetDesktopColor()");//界面字符串显示
	GUI_Delay(SPEED*3/2);
	WM_SetDesktopColor(GUI_BLACK);//设置桌面窗口的颜色
	GUI_Delay(SPEED/2);
	
	GUI_SetBkColor(GUI_BLACK);
	WM_SetDesktopColor(GUI_INVALID_COLOR);
	//GUI_INVALID_COLOR恢复为默认设置
}

//创建窗口的演示函数,主要演示WM_CreateWindow
static void _DemoCreateWindow(void)
{
	_cbBkWindowOld = WM_SetCallback(WM_HBKWIN,sample_cbBkWindow); //设置背景桌面的回调函数
	//创建窗口
	_ChangeInfoText("WM_CreateWindow()");//向背景发送一个装有显示内容的消息，再使得背景层无效
	GUI_Delay(SPEED);
	_hWindow1 = WM_CreateWindow(50,70,165,100,	WM_CF_SHOW|WM_CF_MEMDEV,sample_cbWindow1,0); //创建窗口1，使用自动存储设备
	GUI_Delay(SPEED/3);
	_hWindow2 = WM_CreateWindow(105,125,165,100,WM_CF_SHOW|WM_CF_MEMDEV,_cbWindow2,0); //创建窗口2，使用自动存储设备
	GUI_Delay(SPEED);
}


//创建子窗口，主要演示WM_CreateWindowAsChild()函数
static void _DemoCreateWindowAsChild(void)
{
	_ChangeInfoText("WM_CreateWindowChild()");//向背景发送一个装有显示内容的消息，再使得背景层无效
	GUI_Delay(SPEED);
	_hChild = WM_CreateWindowAsChild(10,50,145,40,_hWindow2,WM_CF_SHOW|WM_CF_MEMDEV,_cbChild,0); //向窗口2创建一个子窗口
	//int x0, int y0,int width, int height,WM_HWIN hWinParent,U8 Style,WM_CALLBACK* cbint NumExtraBytes
	//父窗口在窗口坐标中的左上 X 位置、 Y 位置。窗口尺寸x、y、父窗口句柄、窗口标识、回调指针、分配字节数
	GUI_Delay(SPEED);
}


//让指定的窗口无效，主要演示WM_InvalidateWindow()函数
static void _DemoInvalidateWindow(void)
{
	_ChangeInfoText("WM_InvalidateWindow()");//向背景发送一个装有显示内容的消息，再使得背景层无效
	_WindowColor1 = GUI_BLUE;//重绘前修改颜色
	_FrameColor1 = GUI_GREEN;
	GUI_Delay(SPEED);
	//使窗口1无效，会重绘窗口，注意窗口颜色的变化
	WM_InvalidateWindow(_hWindow1); //会发送窗口重绘msg，在对应设置窗口句柄的回调函数里面调用
	GUI_Delay(SPEED);
}

//将指定窗口放在其他同属窗口的前面，主要展示WMBringToTop()函数的使用
static void _DemoBringToTop(void)
{
	_ChangeInfoText("WM_BringToTop()");
	GUI_Delay(SPEED);
	WM_BringToTop(_hWindow1); //使窗口1放在其他同属窗口的前面
	GUI_Delay(SPEED); 
}

//移动窗口到指定地方，主要展示WM_MoveTo()函数
static void _DemoMoveTo(void)
{
	int i;
	int tm;
	int tDiff;
	
	_ChangeInfoText("WM_MoveTo()");//指定背景无效 重绘，之后显示需要显示的内容
	GUI_Delay(SPEED);
	for(i=1;i<56;i++)
	{
		tm=GUI_GetTime();
		WM_MoveTo(_hWindow1,50+i,70+i);  //移动指定窗口到指定位置
		WM_MoveTo(_hWindow2,105-i,125-i);
		tDiff=15-(GUI_GetTime()-tm);
		GUI_Delay(tDiff);
	}
	for(i=1;i<56;i++)
	{
		tm=GUI_GetTime();
		WM_MoveTo(_hWindow1,105-i,125-i);
		WM_MoveTo(_hWindow2,50+i,70+i);
		tDiff=15-(GUI_GetTime()-tm);
		GUI_Delay(tDiff);
	}
	GUI_Delay(SPEED);
}

//将指定窗口放在其他同属窗口的前面，主要展示WMBringToBottom()函数的使用
static void _DemoBringToBottom(void)
{
	_ChangeInfoText("WM_BringToBottom()");
	GUI_Delay(SPEED);
	WM_BringToBottom(_hWindow1); 	//使窗口1放在其他同属窗口的后面
	GUI_Delay(SPEED);
}


//移动窗口指定的距离，主要展示WM_MoveWindow()函数的使用
//WM_MoveWindow()函数是把窗口移动指定距离
//而WM_MoveTo()是将窗口移动到指定位置,注意两个函数的区别
static void _DemoMoveWindow(void)
{
	int i;
	int tm;
	int tDiff;
	
	_ChangeInfoText("WM_MoveWindow()");
	GUI_Delay(SPEED);
	for(i=0;i<55;i++)
	{
		tm=GUI_GetTime();
		WM_MoveWindow(_hWindow1,1,1); 	//移动窗口1
		WM_MoveWindow(_hWindow2,-1,-1);	//移动窗口2
		//窗口句柄。 要移动的水平距离。 要移动的垂直距离。
		tDiff=15-(GUI_GetTime()-tm);
		GUI_Delay(tDiff);
	}
	for(i=0;i<55;i++)
	{
		tm=GUI_GetTime();
		WM_MoveWindow(_hWindow1,-1,-1);
		WM_MoveWindow(_hWindow2,1,1);
		tDiff=15-(GUI_GetTime()-tm);
		GUI_Delay(tDiff);
	}
	GUI_Delay(SPEED);
}

//隐藏和显示父窗口,主要演示WM_HideWindow()和WM_ShowWindow()函数的使用
//调用WM_HideWindow()或者WM_ShowWindow()这两个函数后并不会立即隐藏或者
//显示窗口，需要执行WM_Exec()命令重绘，如果要立即隐藏或者显示的话应调用
//WM_Paint()函数。GUI_Delay()函数里面调用了WM_Exec()，所以这里我们看到
//的好像是立即隐藏或者显示了窗口
static void _DemoHideShowParent(void)
{
	_ChangeInfoText("WM_HideWindow(Parent)");
	GUI_Delay(SPEED);
	WM_HideWindow(_hWindow2);	//隐藏窗口2
	GUI_Delay(SPEED/3);
	WM_HideWindow(_hWindow1);	//隐藏窗口1
	GUI_Delay(SPEED);
	
	_ChangeInfoText("WM_ShowWindow(Parent)");
	GUI_Delay(SPEED);
	WM_ShowWindow(_hWindow1);	//显示窗口1
	WM_ShowWindow(_hWindow2);	//显示窗口2
	GUI_Delay(SPEED);
	
}

//隐藏和显示子窗口,主要演示WM_HideWindow()和WM_ShowWindow()函数的使用
static void _DemoHideShowChild(void)
{
	_ChangeInfoText("WM_HideWindow(Child)");
	GUI_Delay(SPEED);
	WM_HideWindow(_hChild);	//隐藏子窗口
	GUI_Delay(SPEED);
	
	_ChangeInfoText("WM_ShowWindow(Child)");
	GUI_Delay(SPEED);
	WM_ShowWindow(_hChild);	//显示子窗口
	GUI_Delay(SPEED);
}	

//演示窗口裁剪功能
static void _DemoClipping(void)
{
	int i;
	int tm;
	int tDiff;
	
	_ChangeInfoText("Demostrating clipping of child");
	for(i=0;i<25;i++)
	{
		tm=GUI_GetTime();
		WM_MoveWindow(_hChild,1,0);
		tDiff = 15-(GUI_GetTime()-tm);
		GUI_Delay(tDiff);
	}
	for(i=0;i<25;i++)
	{
		tm=GUI_GetTime();
		WM_MoveWindow(_hChild,0,1);
		tDiff = 15-(GUI_GetTime()-tm);
		GUI_Delay(tDiff);
	}
	for(i=0;i<50;i++)
	{
		tm=GUI_GetTime();
		WM_MoveWindow(_hChild,-1,0);
		tDiff = 15-(GUI_GetTime()-tm);
		GUI_Delay(tDiff);
	}
	for(i=0;i<25;i++)
	{
		tm=GUI_GetTime();
		WM_MoveWindow(_hChild,0,-1);
		tDiff = 15-(GUI_GetTime()-tm);
		GUI_Delay(tDiff);
	}
	for(i=0;i<25;i++)
	{
		tm=GUI_GetTime();
		WM_MoveWindow(_hChild,1,0);
		tDiff = 15-(GUI_GetTime()-tm);
		GUI_Delay(tDiff);
	}
	GUI_Delay(SPEED);
}

//演示使用回调函数来实现窗口的重绘
static void _DemoRedrawing(void)
{
	int i;
	int tm;
	int tDiff;
	
	_ChangeInfoText("Demonstrating redrawing");
	GUI_Delay(SPEED);
	_LiftUp(40);	//上移40个像素
	GUI_Delay(SPEED/3);
	_ChangeInfoText("Using a callback for redrawing");
	GUI_Delay(SPEED/3);
	for (i = 0; i < 55; i++) 
	{
		tm = GUI_GetTime();
		WM_MoveWindow(_hWindow1,  1,  1);
		WM_MoveWindow(_hWindow2, -1, -1);
		tDiff = 15 - (GUI_GetTime() - tm);
		GUI_Delay(tDiff);
	}
	for (i = 0; i < 55; i++) 
	{
		tm = GUI_GetTime();
		WM_MoveWindow(_hWindow1, -1, -1);
		WM_MoveWindow(_hWindow2,  1,  1);
		tDiff = 15 - (GUI_GetTime() - tm);
		GUI_Delay(tDiff);
	}
	GUI_Delay(SPEED/4);
	_LiftDown(30);
	GUI_Delay(SPEED/2);
	_ChangeInfoText("Without redrawing");
	GUI_Delay(SPEED);
	_LiftUp(30);
	GUI_Delay(SPEED/4);
	WM_SetCallback(WM_HBKWIN, _cbBkWindowOld);	//取消背景窗口的回调函数
	for (i = 0; i < 55; i++) 
	{
		tm = GUI_GetTime();
		WM_MoveWindow(_hWindow1,  1,  1);
		WM_MoveWindow(_hWindow2, -1, -1);
		tDiff = 15 - (GUI_GetTime() - tm);
		GUI_Delay(tDiff);
	}
	for (i = 0; i < 55; i++) 
	{
		tm = GUI_GetTime();
		WM_MoveWindow(_hWindow1, -1, -1);
		WM_MoveWindow(_hWindow2,  1,  1);
		tDiff = 15 - (GUI_GetTime() - tm);
		GUI_Delay(tDiff);
	}
	GUI_Delay(SPEED/3);
	WM_SetCallback(WM_HBKWIN, sample_cbBkWindow); //恢复背景窗口的回调函数
	_LiftDown(40);
	GUI_Delay(SPEED);
}

//演示缩放窗口，主要是WM_ResizeWindow()函数的使用
static void _DemoResizeWindow(void)
{
	int i;
	int tm;
	int tDiff;
	
	_ChangeInfoText("WM_ResizeWindow()");
	GUI_Delay(SPEED);
	_LiftUp(30);
	for(i=0;i<20;i++)
	{
		tm=GUI_GetTime();
		WM_ResizeWindow(_hWindow1,1,1); //窗口1的X,Y轴每次增加1个像素大小
		WM_ResizeWindow(_hWindow2,-1,-1);//窗口2的X,Y轴每次减小一个像素大小
		tDiff=15-(GUI_GetTime()-tm);
		GUI_Delay(tDiff);
	}
	for (i = 0;i < 40; i++) 
	{
		tm = GUI_GetTime();
		WM_ResizeWindow(_hWindow1, -1, -1);
		WM_ResizeWindow(_hWindow2,  1,  1);
		tDiff = 15 - (GUI_GetTime() - tm);
		GUI_Delay(tDiff);
	}
	for (i = 0; i < 20; i++)
	{
		tm = GUI_GetTime();
		WM_ResizeWindow(_hWindow1,  1,  1);
		WM_ResizeWindow(_hWindow2, -1, -1);
		tDiff = 15 - (GUI_GetTime() - tm);
		GUI_Delay(tDiff);
	}
	_LiftDown(30);
	GUI_Delay(SPEED);
}


//设置回调函数，主要演示WM_SetCallback()函数的使用
static void _DemoSetCallback(void)
{
	_ChangeInfoText("WM_SetCallback()");
	WM_SetCallback(_hWindow1, _cbDemoCallback1);//重新绘制回调函数
	WM_InvalidateWindow(_hWindow1);//界面无效 使用新的回调函数重绘
	GUI_Delay(SPEED/2);
	WM_SetCallback(_hWindow2, _cbDemoCallback2);
	WM_InvalidateWindow(_hWindow2);
	GUI_Delay(SPEED*3);
	WM_SetCallback(_hWindow1, sample_cbWindow1);//重新设置原来的回调函数
	WM_InvalidateWindow(_hWindow1);
	GUI_Delay(SPEED/2);
	WM_SetCallback(_hWindow2, _cbWindow2);
	WM_InvalidateWindow(_hWindow2);
	GUI_Delay(SPEED);
}

//删除窗口
static void _DemoDeleteWindow(void)
{
	_ChangeInfoText("WM_DeleteWindow()");
	GUI_Delay(SPEED);
	WM_DeleteWindow(_hWindow2); //删除窗口2
	GUI_Delay(SPEED/3);
	WM_DeleteWindow(_hWindow1);	//删除窗口1
	GUI_Delay(SPEED);
	_ChangeInfoText("");
	GUI_Delay(SPEED);

	//恢复窗口颜色和回调函数
	WM_SetCallback(WM_HBKWIN, _cbBkWindowOld);
	_WindowColor1 = GUI_GREEN;
	_WindowColor2 = GUI_RED;
}


//演示函数
void sample_ShowDemo(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);	// 使能CRC时钟，用于emwin验证	
	GUI_Init();
	WM_MULTIBUF_Enable(1);	// 开启多缓冲	
	GUI_SetBkColor(GUI_BLACK);//设置背景颜色
	while (1) 
	{
		_DemoSetDesktopColor();//桌面窗口颜色设置和恢复
		_DemoCreateWindow();//创建两个窗口 window1和window2
		_DemoCreateWindowAsChild();//创建子窗口，基于windows2
		_DemoInvalidateWindow();//window1设置无效，使得窗口1实现重绘
		_DemoBringToTop();//使窗口1放在其他同属窗口的前面
		_DemoMoveTo();
		_DemoBringToBottom();//使窗口1放在其他同属窗口的后面
		_DemoMoveWindow();
		_DemoHideShowParent();//隐藏窗口和显示窗口
		_DemoHideShowChild();//隐藏子窗口和显示子窗口
		_DemoClipping();//演示窗口裁剪功能 子窗口超过父窗口的一部分将会被剪切掉
		_DemoRedrawing();//演示回调函数与没有回调函数的区别
		_DemoResizeWindow();//窗口扩大和缩小
		_DemoSetCallback();//修改回调函数后设置窗口无效来实现让窗口显示别的
		_DemoDeleteWindow();
  }
}






