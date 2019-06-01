#include "board.h"
#include "gpio.h"
#include "timer.h"

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
#define RT_HEAP_SIZE 0x00090000 //max：0x00100000
static uint8_t *rt_heap=NULL;	
RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif


/**
 * This function will initial your board.
 */
void rt_hw_board_init()
{	
	SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);
	GPIOInit();
	TIMx_Configuration();
	Usart_Config();
	IOE_Config();
	rt_heap=malloc(RT_HEAP_SIZE);
	cpu_usage_init();
	RTC_CLK_Config();
	RTC_TimeAndDate_Set();
	
    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif
    
#if defined(RT_USING_CONSOLE) && defined(RT_USING_DEVICE)
	rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif
    
#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
}

extern u32 TimingDelay;//delay.c中通过记录中断次数达到延时
 /**
 * @brief SysTick 中断服务函数
 * @param 无
 * @retval 无
 *
 * @attention
 * SysTick 中断服务函数在固件库文件 stm32f10x_it.c 中也定义了，而现在
 * 在 board.c 中又定义一次，那么编译的时候会出现重复定义的错误，解决
 * 方法是可以把 stm32f10x_it.c 中的注释或者删除即可。
 */
void SysTick_Handler(void)  
{
	/* 进入中断 */
	rt_interrupt_enter();
	if (TimingDelay != 0x00) 
	{
		TimingDelay--;
	}
	/* 更新时基 */
	rt_tick_increase();
	/* 离开中断 */
	rt_interrupt_leave();
}

/**
* @brief 重映射串口 DEBUG_USARTx 到 rt_kprintf()函数
* Note： DEBUG_USARTx 是在 bsp_usart.h 中定义的宏，默认使用串口 1
* @param str：要输出到串口的字符串
* @retval 无
*
* @attention
*
*/
void rt_hw_console_output(const char *str)
{
	/* 进入临界段 */
 rt_enter_critical();

 /* 直到字符串结束 */
 while (*str!='\0')
 {
 /* 换行 */
	if (*str=='\n')
 {
	 USART_SendData(DEBUGUSART, '\r');
	 while (USART_GetFlagStatus(DEBUGUSART, USART_FLAG_TXE) == RESET);
	 }

	 USART_SendData(DEBUGUSART, *str++);
	 while (USART_GetFlagStatus(DEBUGUSART, USART_FLAG_TXE) == RESET);
	 }
	 /* 退出临界段 */
	 rt_exit_critical();
}

