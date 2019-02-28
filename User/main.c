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
	rt_thread_init(&Thread1TCB, /* �߳̿��ƿ� */
									"Thread1", /* �߳����� */
									Thread1, /* �߳���ں��� */
									RT_NULL, /* �߳���ں������� */
									&Thread1Stk[0], /* �߳�ջ��ʼ��ַ */
									sizeof(Thread1Stk), /* �߳�ջ��С */
									3, /* �̵߳����ȼ� */
									100); /* �߳�ʱ��Ƭ */									
	rt_thread_startup(&Thread1TCB); /* �����̣߳��������� */	
	rt_thread_init(&Thread2TCB, /* �߳̿��ƿ� */
									"Thread2", /* �߳����� */
									Thread2, /* �߳���ں��� */
									RT_NULL, /* �߳���ں������� */
									&Thread2Stk[0], /* �߳�ջ��ʼ��ַ */
									sizeof(Thread2Stk), /* �߳�ջ��С */
									3, /* �̵߳����ȼ� */
									100); /* �߳�ʱ��Ƭ */
	rt_thread_startup(&Thread2TCB); /* �����̣߳��������� */

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
	_DemoRedraw();  //�����ػ���Ժ���
//	_ShowDemo();
}



