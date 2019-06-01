#include "board.h"
#include "gpio.h"
#include "timer.h"

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
#define RT_HEAP_SIZE 0x00090000 //max��0x00100000
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

extern u32 TimingDelay;//delay.c��ͨ����¼�жϴ����ﵽ��ʱ
 /**
 * @brief SysTick �жϷ�����
 * @param ��
 * @retval ��
 *
 * @attention
 * SysTick �жϷ������ڹ̼����ļ� stm32f10x_it.c ��Ҳ�����ˣ�������
 * �� board.c ���ֶ���һ�Σ���ô�����ʱ�������ظ�����Ĵ��󣬽��
 * �����ǿ��԰� stm32f10x_it.c �е�ע�ͻ���ɾ�����ɡ�
 */
void SysTick_Handler(void)  
{
	/* �����ж� */
	rt_interrupt_enter();
	if (TimingDelay != 0x00) 
	{
		TimingDelay--;
	}
	/* ����ʱ�� */
	rt_tick_increase();
	/* �뿪�ж� */
	rt_interrupt_leave();
}

/**
* @brief ��ӳ�䴮�� DEBUG_USARTx �� rt_kprintf()����
* Note�� DEBUG_USARTx ���� bsp_usart.h �ж���ĺ꣬Ĭ��ʹ�ô��� 1
* @param str��Ҫ��������ڵ��ַ���
* @retval ��
*
* @attention
*
*/
void rt_hw_console_output(const char *str)
{
	/* �����ٽ�� */
 rt_enter_critical();

 /* ֱ���ַ������� */
 while (*str!='\0')
 {
 /* ���� */
	if (*str=='\n')
 {
	 USART_SendData(DEBUGUSART, '\r');
	 while (USART_GetFlagStatus(DEBUGUSART, USART_FLAG_TXE) == RESET);
	 }

	 USART_SendData(DEBUGUSART, *str++);
	 while (USART_GetFlagStatus(DEBUGUSART, USART_FLAG_TXE) == RESET);
	 }
	 /* �˳��ٽ�� */
	 rt_exit_critical();
}

