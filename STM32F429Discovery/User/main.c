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
#include "timer.h"
#include "esp8266.h"
#include "gpio.h"

uint8_t Data[100];
extern uint8_t USART1_RX_BUFF[512];
extern uint32_t USART1_RX_CNT;
extern uint32_t USART1_RX_FLAG;

const uint8_t weatherName[3][7][20]=
{
	{"Cloudy","Over cast"},
	{"Shower","Heavy rain","Rain storm","Moderate rain","Thunder shower","Light rain"},
	{"sun"}
};

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
struct rt_thread SamplingTCB;

ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t UartStk[2048] __EXRAM ;
static void Usart(void* parameter);
struct rt_thread UsartTCB;



rt_uint8_t DHT11_temperature;  	    
rt_uint8_t DHT11_humidity;
short DS18B20_temperature1=0,DS18B20_temperature2=0,DS18B20_temperature3=0,DS18B20_temperature4=0;

int main(void)
{	
	printf("clk:%d\n",SystemCoreClock);
	rt_thread_init(&Thread1TCB, /* 线程控制块 */
									"Touch", /* 线程名字 */
									Thread1, /* 线程入口函数 */
									RT_NULL, /* 线程入口函数参数 */
									&Thread1Stk[0], /* 线程栈起始地址 */
									sizeof(Thread1Stk), /* 线程栈大小 */
									4, /* 线程的优先级 */
									40); /* 线程时间片 */									
	rt_thread_startup(&Thread1TCB); /* 启动线程，开启调度 */	
	rt_thread_init(&Thread2TCB, /* 线程控制块 */
									"STemwin", /* 线程名字 */
									Thread2, /* 线程入口函数 */
									RT_NULL, /* 线程入口函数参数 */
									&Thread2Stk[0], /* 线程栈起始地址 */
									sizeof(Thread2Stk), /* 线程栈大小 */
									4, /* 线程的优先级 */
									40); /* 线程时间片 */
	rt_thread_startup(&Thread2TCB); /* 启动线程，开启调度 */
	rt_thread_init(&SamplingTCB, /* 线程控制块 */
									"Sample", /* 线程名字 */
									Sampling, /* 线程入口函数 */
									RT_NULL, /* 线程入口函数参数 */
									&SamplingStk[0], /* 线程栈起始地址 */
									sizeof(SamplingStk), /* 线程栈大小 */
									3, /* 线程的优先级 */
									40); /* 线程时间片 */
	rt_thread_startup(&SamplingTCB); /* 启动线程，开启调度 */
	rt_thread_init(&UsartTCB, /* 线程控制块 */
									"Usart", /* 线程名字 */
									Usart, /* 线程入口函数 */
									RT_NULL, /* 线程入口函数参数 */
									&UartStk[0], /* 线程栈起始地址 */
									sizeof(UartStk), /* 线程栈大小 */
									3, /* 线程的优先级 */
									40); /* 线程时间片 */
	rt_thread_startup(&UsartTCB); /* 启动线程，开启调度 */

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
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);	// 使能CRC时钟
	WM_SetCreateFlags(WM_CF_MEMDEV); //启动所有窗口的存储设备
	GUI_Init();  			//STemWin初始化
	WM_MULTIBUF_Enable(1);  //开启STemWin多缓冲,RGB屏可能会用到
	k_StartUp();//开机动画
	InitSystemConfigStruct();//初始化设置信息结构体变量
	iconviewdemo(); //桌面显示
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
	while(DS18B20_Init())	//DS18B20初始化	
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

static void Usart(void* parameter)
{
	ESP8266_STATCPClient_MODE_Init();
	while(1)
	{
		if(USART1_RX_FLAG)//收到一帧完整的数据
		{
			USART1_RX_FLAG=0;//标志位归位
			USART1_RX_BUFF[USART1_RX_CNT]='\0';//添加结束符
			printf("usart1 resive :%s \n",USART1_RX_BUFF);
			USART1_RX_CNT=0;//计数清零
//			if(strstr((char *)USART1_RX_BUFF,"NoPassWord")!=NULL)
//			{
//				Usart_SendString(USART1,"setID:1234");
//			}
//			else
			if(strstr((char *)USART1_RX_BUFF,"GetTemp")!=NULL)
			{
				sprintf((char *)Data,"Temp:T1:%dT2:%dT3:%dT4:%dendT",DS18B20_temperature1,DS18B20_temperature2,DS18B20_temperature3,DS18B20_temperature4);
				Usart_SendString(USART1,(char *)Data);
			}
			else
			if(strstr((char *)USART1_RX_BUFF,"GetHum")!=NULL)
			{
				sprintf((char *)Data,"Temp:Hum:%dendH",DHT11_humidity);
				Usart_SendString(USART1,(char *)Data);
			}
			else
			if(strstr((char *)USART1_RX_BUFF,"GetPM")!=NULL)
			{
				sprintf((char *)Data,"Temp:PM:%dendP",55);
				Usart_SendString(USART1,(char *)Data);
			}			
			else
			if(strstr((char *)USART1_RX_BUFF,"SetS:10")!=NULL)
			{
				printf("1off\n");
				PORT1=0;
			}			
			else
			if(strstr((char *)USART1_RX_BUFF,"SetS:11")!=NULL)
			{
				printf("1on\n");
				PORT1=1;
			}			
			else
			if(strstr((char *)USART1_RX_BUFF,"SetS:20")!=NULL)
			{
				printf("2off\n");
				PORT2=0;
			}			
			else
			if(strstr((char *)USART1_RX_BUFF,"SetS:21")!=NULL)
			{
				printf("2on\n");
				PORT2=1;
			}			
			else
			if(strstr((char *)USART1_RX_BUFF,"SetS:30")!=NULL)
			{
				printf("3off\n");
				PORT3=0;
			}			
			else
			if(strstr((char *)USART1_RX_BUFF,"SetS:31")!=NULL)
			{
				printf("3on\n");
				PORT3=1;
			}			
			else
			if(strstr((char *)USART1_RX_BUFF,"SetS:40")!=NULL)
			{
				printf("4off\n");
				PORT4=0;
			}			
			else
			if(strstr((char *)USART1_RX_BUFF,"SetS:41")!=NULL)
			{
				printf("4on\n");
				PORT4=1;
			}			
			else
			if(strstr((char *)USART1_RX_BUFF,"GetSwitch")!=NULL)
			{
				sprintf((char *)Data,"SSW:%d%d%d%d",(int)PORT1,(int)PORT2,(int)PORT3,(int)PORT4);
				Usart_SendString(USART1,(char *)Data);
			}
			else
			if(strstr((char *)USART1_RX_BUFF,"SetTime")!=NULL)
			{
				RTC_DateTypeDef RTC_DateStructure;
				RTC_TimeTypeDef RTC_TimeStructure;
				char *p=NULL;
				char date[11],time[9];
				char year[5],mouth[3],day[3],hour[3],min[3],sec[3];
				int y,m,d,h,mi,s;
				p=strstr((char *)USART1_RX_BUFF,"SetTime");
				strncpy(date,p+8,10);
				p=strstr((char *)USART1_RX_BUFF,"end");
				strncpy(time,p-8,8);		
				strncpy(year,date,4);	
				strncpy(mouth,date+5,2);	
				strncpy(day,date+8,2);		
				strncpy(hour,time,2);		
				strncpy(min,time+3,2);		
				strncpy(sec,time+6,2);		
				y=atoi(year);	
				m=atoi(mouth);			
				d=atoi(day);		
				h=atoi(hour);					
				mi=atoi(min);	
				s=atoi(sec);	
				
				
				RTC_TimeStructure.RTC_Seconds =s;
				RTC_TimeStructure.RTC_Minutes =mi; 
				RTC_TimeStructure.RTC_Hours =h;
				RTC_TimeStructure.RTC_H12 = RTC_H12_AMorPM;     
				RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure);
				RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
				RTC_DateStructure.RTC_WeekDay = 0;       
				RTC_DateStructure.RTC_Date =d;         
				RTC_DateStructure.RTC_Month = m;         
				RTC_DateStructure.RTC_Year =y-2000;        
				RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStructure);
				RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
				
				
//				printf("data:%s,time:%s\n",date,time);
//				printf("%s-%s-%s %s:%s:%s\n",year,mouth,day,hour,min,sec);
//				printf("%d-%d-%d %d:%d:%d\n",y,m,d,h,mi,s);
			}				
			else
			if(strstr((char *)USART1_RX_BUFF,"SetWeather")!=NULL)
			{
				char *p=NULL;
				char date[8],wea[3],TEMPH[3],TEMPL[3],tempL,tempH;
				p=strstr((char *)USART1_RX_BUFF,"SetWeather");
				strncpy(date,p+11,7);
				strncpy(wea,date,2);		
				strncpy(TEMPH,date+3,2);	
				strncpy(TEMPL,date+5,2);		
//				printf("wes:%s\n",date);
				tempH=atoi(TEMPH);	
				tempL=atoi(TEMPL);			
//				printf("wea:%s,templ:%s=%d,temph:%s=%d\n",wea,TEMPH,tempL,TEMPL,tempH);
				switch(wea[0])
				{
					case 'A':
						switch(wea[1])
						{
							case '1':printf("weather:%s\n",weatherName[0][0]);break;
							case '2':printf("weather:%s\n",weatherName[0][1]);break;
						}
						break;
					case 'B':
						switch(wea[1])
						{
							case '1':printf("weather:%s\n",weatherName[1][0]);break;
							case '2':printf("weather:%s\n",weatherName[1][1]);break;
							case '3':printf("weather:%s\n",weatherName[1][2]);break;
							case '4':printf("weather:%s\n",weatherName[1][3]);break;
							case '5':printf("weather:%s\n",weatherName[1][4]);break;
							case '6':printf("weather:%s\n",weatherName[1][5]);break;
						}
						break;
					case 'C':
						switch(wea[1])
						{
							case '1':printf("weather:%s\n",weatherName[2][0]);break;
						}
						break;
				}
			}			
		}		
		rt_thread_delay(50);
	}

}



