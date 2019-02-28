#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "sdram.h"
#include "stm32f429i_discovery_ioe.h"
#include "board.h"
#include "rtthread.h"
#include "arm_math.h"
#include "GUI.h"
#include "redraw.h"
#include "sample.h"
#include "WM.h"


struct rt_thread Thread1TCB;
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t Thread1Stk[1024] __EXRAM ;
static void Thread1(void* parameter);

struct rt_thread Thread2TCB;
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t Thread2Stk[4096] __EXRAM ;
static void Thread2(void* parameter);

int main(void)
{	
	rt_thread_init(&Thread1TCB, /* 线程控制块 */
									"Thread1", /* 线程名字 */
									Thread1, /* 线程入口函数 */
									RT_NULL, /* 线程入口函数参数 */
									&Thread1Stk[0], /* 线程栈起始地址 */
									sizeof(Thread1Stk), /* 线程栈大小 */
									3, /* 线程的优先级 */
									100); /* 线程时间片 */									
	rt_thread_startup(&Thread1TCB); /* 启动线程，开启调度 */	
	rt_thread_init(&Thread2TCB, /* 线程控制块 */
									"Thread2", /* 线程名字 */
									Thread2, /* 线程入口函数 */
									RT_NULL, /* 线程入口函数参数 */
									&Thread2Stk[0], /* 线程栈起始地址 */
									sizeof(Thread2Stk), /* 线程栈大小 */
									3, /* 线程的优先级 */
									100); /* 线程时间片 */
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
	_DemoRedraw();  //窗口重绘测试函数
//	_ShowDemo();
}



