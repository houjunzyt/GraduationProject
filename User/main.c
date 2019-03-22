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
	rt_thread_init(&Thread1TCB, /* �߳̿��ƿ� */
									"Touch", /* �߳����� */
									Thread1, /* �߳���ں��� */
									RT_NULL, /* �߳���ں������� */
									&Thread1Stk[0], /* �߳�ջ��ʼ��ַ */
									sizeof(Thread1Stk), /* �߳�ջ��С */
									3, /* �̵߳����ȼ� */
									40); /* �߳�ʱ��Ƭ */									
	rt_thread_startup(&Thread1TCB); /* �����̣߳��������� */	
	rt_thread_init(&Thread2TCB, /* �߳̿��ƿ� */
									"STemwin", /* �߳����� */
									Thread2, /* �߳���ں��� */
									RT_NULL, /* �߳���ں������� */
									&Thread2Stk[0], /* �߳�ջ��ʼ��ַ */
									sizeof(Thread2Stk), /* �߳�ջ��С */
									3, /* �̵߳����ȼ� */
									40); /* �߳�ʱ��Ƭ */
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
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);	// ʹ��CRCʱ��
	WM_SetCreateFlags(WM_CF_MEMDEV); //�������д��ڵĴ洢�豸
	GUI_Init();  			//STemWin��ʼ��
	WM_MULTIBUF_Enable(1);  //����STemWin�໺��,RGB�����ܻ��õ�
	k_StartUp();//��������
	iconviewdemo(); //������ʾ
	while(1)
	{
		GUI_Exec();  
		rt_thread_delay(20);
		GUI_FreeMem = GUI_ALLOC_GetNumFreeBytes();
	}
}

