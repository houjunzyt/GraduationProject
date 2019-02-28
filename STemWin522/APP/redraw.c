#include "redraw.h"
#include "WM.h"
#include "lcd_user.h"	

//背景窗口回调函数
static void _cbBkWindow(WM_MESSAGE *pMsg)
{
	switch(pMsg->MsgId) //根据不同的消息类型处理
	{
		case WM_PAINT:   //WM_PAIMNT消息，窗口重绘自身
			GUI_ClearRect(0,50,LCD_X_LEN-1,LCD_Y_LEN-1);//y坐标50以下全部填充背景色
			break;
		default:
			WM_DefaultProc(pMsg); //此函数可处理未处理的消息
	}
}

//窗口回调函数
static void _cbWindow(WM_MESSAGE *pMsg)
{
	GUI_RECT Rect;
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
			WM_GetInsideRect(&Rect); //返回活动窗口减去边界后的尺寸，获取绘制的窗口信息
			GUI_SetBkColor(GUI_RED); //红色背景色
			GUI_SetColor(GUI_YELLOW);//黄色
			GUI_ClearRectEx(&Rect);	 //清除Rect区域,既绘制红色区域
			GUI_DrawRectEx(&Rect);	 //绘制Rect区域,既绘制黄色框框，绘制矩形
			GUI_SetColor(GUI_BLACK); //黑色
			GUI_SetFont(&GUI_Font16_ASCII); //设置字体
			GUI_DispStringHCenterAt("Foreground Window",75,40);
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}

//移动窗口
static void _MoveWindow(const char* pText)
{
	WM_HWIN hWnd;
	int i;
	//创建前景窗口
	hWnd = WM_CreateWindow(10,50,150,100,WM_CF_SHOW,_cbWindow,0);//窗口的大小数据会传输给窗口绘制回调函数Rect，再绘制窗口
	GUI_Delay(500);
	
	//移动前景窗口
	for(i=0;i<40;i++)
	{
		WM_MoveWindow(hWnd,2,2); //移动窗口
		GUI_Delay(10);
	}
	
	//删除窗口之前显示文本
	if(pText)
	{
		GUI_DispStringAt(pText,5,50);
		GUI_Delay(2500);
	}
	
	//删除前景窗口
	WM_DeleteWindow(hWnd); //删除窗口
	WM_Invalidate(WM_HBKWIN); //背景窗口无效
	GUI_Exec();
}


//窗口演示
void _DemoRedraw(void)//新建个线程直接调用这个便可
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);	// 使能CRC时钟，用于emwin验证	
	GUI_Init();
	WM_MULTIBUF_Enable(1);	// 开启多缓冲	
	WM_CALLBACK *_cbOldBK;
	GUI_SetBkColor(GUI_BLACK); 	//设置背景颜色
	GUI_Clear();
	GUI_SetColor(GUI_WHITE);	//白色
	GUI_SetFont(&GUI_Font24_ASCII);//设置字体
	GUI_DispStringHCenterAt("WM_Redraw",LCD_X_LEN/2,5);//居中显示
	GUI_SetFont(&GUI_Font16B_ASCII); //设置字体
	while(1)
	{
		_MoveWindow("Background has not been redraw"); //移动窗口
		
		//清除背景
		GUI_ClearRect(0,50,LCD_X_LEN-1,LCD_Y_LEN-1);//y坐标50以下全部填充背景色
		GUI_Delay(1000);
		
		//为背景窗口设置回调函数
		_cbOldBK = WM_SetCallback(WM_HBKWIN,_cbBkWindow);
		
		//移动窗口
		_MoveWindow("Background has been redraw");
		
		//窗口WM_HBKWIN的回调函数重新设置为上一个回调函数
		WM_SetCallback(WM_HBKWIN,_cbOldBK);		
	}
}







