#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "sdram.h"
#include "stm32f429i_discovery_ioe.h"
#include "board.h"
#include "rtthread.h"
#include "arm_math.h"
#include "GUI.h"
#include "DIALOG.h"
#include "header.h"
#include "WM.h"
#include "iconviewdemo.h"

uint32_t GUI_FreeMem = 0;
struct rt_thread Thread1TCB;
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t Thread1Stk[1024] __EXRAM ;
static void Thread1(void* parameter);

struct rt_thread Thread2TCB;
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t Thread2Stk[8192] __EXRAM ;
static void Thread2(void* parameter);

int main(void)
{	
	printf("clk:%d\n",SystemCoreClock);
	rt_thread_init(&Thread1TCB, /* 线程控制块 */
									"Touch", /* 线程名字 */
									Thread1, /* 线程入口函数 */
									RT_NULL, /* 线程入口函数参数 */
									&Thread1Stk[0], /* 线程栈起始地址 */
									sizeof(Thread1Stk), /* 线程栈大小 */
									3, /* 线程的优先级 */
									40); /* 线程时间片 */									
	rt_thread_startup(&Thread1TCB); /* 启动线程，开启调度 */	
	rt_thread_init(&Thread2TCB, /* 线程控制块 */
									"STemwin", /* 线程名字 */
									Thread2, /* 线程入口函数 */
									RT_NULL, /* 线程入口函数参数 */
									&Thread2Stk[0], /* 线程栈起始地址 */
									sizeof(Thread2Stk), /* 线程栈大小 */
									3, /* 线程的优先级 */
									40); /* 线程时间片 */
	rt_thread_startup(&Thread2TCB); /* 启动线程，开启调度 */

}

static void	Thread1(void *parameter)
{
	rt_kprintf("thread1\n");
	while(1)
	{
		Pointer_Update();
		rt_thread_delay(20);
	}	
}

static void	Thread2(void *parameter)
{	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);	// 使能CRC时钟
	WM_SetCreateFlags(WM_CF_MEMDEV); //启动所有窗口的存储设备
	GUI_Init();  			//STemWin初始化
	WM_MULTIBUF_Enable(1);  //开启STemWin多缓冲,RGB屏可能会用到
	k_StartUp();//开机动画
	iconviewdemo(); //桌面显示
	while(1)
	{
		GUI_Exec();  
		rt_thread_delay(20);
		GUI_FreeMem = GUI_ALLOC_GetNumFreeBytes();
	}
}

