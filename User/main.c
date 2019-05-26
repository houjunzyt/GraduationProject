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
#include "dht11.h"
#include "ds18b20.h"

uint32_t GUI_FreeMem = 0;
struct rt_thread Thread1TCB;
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t Thread1Stk[1024] __EXRAM ;
static void Thread1(void* parameter);

struct rt_thread Thread2TCB;
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t Thread2Stk[8192] __EXRAM ;
static void Thread2(void* parameter);

struct rt_thread SamplingTCB;
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t SamplingStk[1024] __EXRAM ;
static void Sampling(void* parameter);

rt_uint8_t DHT11_temperature;  	    
rt_uint8_t DHT11_humidity;
short DS18B20_temperature1=0,DS18B20_temperature2=0,DS18B20_temperature3=0,DS18B20_temperature4=0;

int main(void)
{	
	printf("clk:%d\n",SystemCoreClock);
	rt_thread_init(&Thread1TCB, /* �߳̿��ƿ� */
									"Touch", /* �߳����� */
									Thread1, /* �߳���ں��� */
									RT_NULL, /* �߳���ں������� */
									&Thread1Stk[0], /* �߳�ջ��ʼ��ַ */
									sizeof(Thread1Stk), /* �߳�ջ��С */
									4, /* �̵߳����ȼ� */
									40); /* �߳�ʱ��Ƭ */									
	rt_thread_startup(&Thread1TCB); /* �����̣߳��������� */	
	rt_thread_init(&Thread2TCB, /* �߳̿��ƿ� */
									"STemwin", /* �߳����� */
									Thread2, /* �߳���ں��� */
									RT_NULL, /* �߳���ں������� */
									&Thread2Stk[0], /* �߳�ջ��ʼ��ַ */
									sizeof(Thread2Stk), /* �߳�ջ��С */
									4, /* �̵߳����ȼ� */
									40); /* �߳�ʱ��Ƭ */
	rt_thread_startup(&Thread2TCB); /* �����̣߳��������� */
	rt_thread_init(&SamplingTCB, /* �߳̿��ƿ� */
									"Sample", /* �߳����� */
									Sampling, /* �߳���ں��� */
									RT_NULL, /* �߳���ں������� */
									&SamplingStk[0], /* �߳�ջ��ʼ��ַ */
									sizeof(SamplingStk), /* �߳�ջ��С */
									3, /* �̵߳����ȼ� */
									40); /* �߳�ʱ��Ƭ */
	rt_thread_startup(&SamplingTCB); /* �����̣߳��������� */

}

static void	Thread1(void *parameter)
{
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
	InitSystemConfigStruct();//��ʼ��������Ϣ�ṹ�����
	iconviewdemo(); //������ʾ
	while(1)
	{
		GUI_Exec();  
		rt_thread_delay(20);
		GUI_FreeMem = GUI_ALLOC_GetNumFreeBytes();
	}
}

static void Sampling(void* parameter)
{
	DWT_Delay_Init();
	while(DHT11_Init())
	{
		rt_kprintf("DHT11 Init Error!");
		rt_thread_delay(1000);
	}
	while(DS18B20_Init())	//DS18B20��ʼ��	
	{
		rt_kprintf("DS18B20 Init Error!");
		rt_thread_delay(1000);
	} 
	while(1)
	{
		DHT11_Read_Data(&DHT11_temperature,&DHT11_humidity);
		DS18B20_temperature1=DS18B20_Get_Temp(1);
		DS18B20_temperature2=DS18B20_Get_Temp(2);
		DS18B20_temperature3=DS18B20_Get_Temp(3);
		DS18B20_temperature4=DS18B20_Get_Temp(4);
		rt_kprintf("DS18B20:%d %d %d %d hum:%d %d\n",DS18B20_temperature1,DS18B20_temperature2,DS18B20_temperature3,DS18B20_temperature4,DHT11_humidity,DHT11_temperature);
		rt_thread_delay(400);
	}
}
