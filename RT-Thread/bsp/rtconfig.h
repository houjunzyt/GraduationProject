
/* RT-Thread config file */

#ifndef __RTTHREAD_CFG_H__
#define __RTTHREAD_CFG_H__

//#include "RTE_Components.h"

// <<< Use Configuration Wizard in Context Menu >>>
// <h>Basic Configuration
// <o>Maximal level of thread priority <8-256>
//	<i>Default: 32
#define RT_THREAD_PRIORITY_MAX     32  //�߳����ȼ�8-256

// <o>OS tick per second
//  <i>Default: 1000   (1ms)
#define RT_TICK_PER_SECOND      1000  //����ϵͳÿ����ٸ�tick

// <o>Alignment size for CPU architecture data access
//	<i>Default: 4
#define RT_ALIGN_SIZE 4  //cpu�ֽڶ��䷽ʽ

// <o>the max length of object name<2-16>
//	<i>Default: 8
#define RT_NAME_MAX 8  //����������󳤶� ȡֵ��Χ��2~16

// <c1>Using RT-Thread components initialization
//  <i>Using RT-Thread components initialization
#define RT_USING_COMPONENTS_INIT   //�Ƿ�ʹ��RTT�����ʼ��
// </c>

// <c1>Using user main
//  <i>Using user main
#define RT_USING_USER_MAIN   //ʹ���û�main����
// </c>

// <o>the size of main thread<1-4086>
//	<i>Default: 512
#define RT_MAIN_THREAD_STACK_SIZE      512   //�߳�ջ��С��1-4086

// <c1>using tiny size of memory
//  <i>using tiny size of memory
//#define RT_USING_TINY_SIZE
// </c>
// </h>

// <h>Debug Configuration      //����ʹ�ã��ں˵������ã�����������á�ջ������
// <c1>enable kernel debug configuration
//  <i>Default: enable kernel debug configuration
//#define RT_DEBUG
// </c>

// <o>enable components initialization debug configuration<0-1>
//  <i>Default: 0
#define RT_DEBUG_INIT 0

// <c1>thread stack over flow detect
//  <i> Diable Thread stack over flow detect
//#define RT_USING_OVERFLOW_CHECK
// </c>
// </h>

// <h>Hook Configuration      //���Ӻ�������  
// <c1>using hook
//  <i>using hook
#define RT_USING_HOOK
// </c>

// <c1>using idle hook
//  <i>using idle hook
#define RT_USING_IDLE_HOOK
// </c>
// </h>

// <e>Software timers Configuration     //�����ʱ������
// <i> Enables user timers
//#define RT_USING_TIMER_SOFT
#define RT_USING_TIMER_SOFT 0
#if RT_USING_TIMER_SOFT == 0
#undef RT_USING_TIMER_SOFT
#endif
// <o>The priority level of timer thread <0-31>
//  <i>Default: 4
#define RT_TIMER_THREAD_PRIO 4

// <o>The stack size of timer thread <0-8192>
//  <i>Default: 512
#define RT_TIMER_THREAD_STACK_SIZE 512

// <o>The soft-timer tick per second <0-1000>
//  <i>Default: 100
#define RT_TIMER_TICK_PER_SECOND 100
// </e>

// <h>IPC(Inter-process communication) Configuration     //�ڲ�ͨ������
// <c1>Using Semaphore
//  <i>Using Semaphore
#define RT_USING_SEMAPHORE     //�ź�
// </c>

// <c1>Using Mutex
//  <i>Using Mutex 
#define RT_USING_MUTEX      //������
// </c>

// <c1>Using Event
//  <i>Using Event
//#define RT_USING_EVENT        //�¼�
// </c>

// <c1>Using MailBox
//  <i>Using MailBox
#define RT_USING_MAILBOX     //����
// </c>

// <c1>Using Message Queue
//  <i>Using Message Queue
#define RT_USING_MESSAGEQUEUE   //��Ϣ����
// </c>
// </h>

// <h>Memory Management Configuration    //�ڴ��������
// <c1>Using Memory Pool Management
//  <i>Using Memory Pool Management    
//#define RT_USING_MEMPOOL          //�Ƿ�ʹ���ڴ��
// </c>
// <c1>Dynamic Heap Management
//  <i>Dynamic Heap Management
#define RT_USING_HEAP      //�Ƿ�ʹ�öѿռ�
// </c>
// <c1>using small memory
//  <i>using small memory
#define RT_USING_SMALL_MEM    //�Ƿ�ʹ��С�ڴ�
// </c>
// </h>

// <h>Console Configuration   //����̨����
// <c1>Using console
//  <i>Using console
#define RT_USING_CONSOLE
// </c>

// <o>the buffer size of console <1-1024>
//  <i>the buffer size of console
//  <i>Default: 128  (128Byte)
#define RT_CONSOLEBUF_SIZE      128

#define DEBUGUSART  USART1

// <s>The device name for console
//  <i>The device name for console
//  <i>Default: uart2
#define RT_CONSOLE_DEVICE_NAME "uart2"
// </h>

#if defined(RTE_USING_DEVICE)    
#define RT_USING_DEVICE
#define RT_USING_SERIAL
#define BSP_USING_UART2
#endif

#if defined(RTE_USING_FINSH)

// <h>Finsh Configuration
// <c1>Using FinSh Shell
//  <i>Using FinSh Shell
#define RT_USING_FINSH
// </c>

// <c1>Using Msh Shell
//  <i>Using Msh Shell
#define FINSH_USING_MSH       //Finsh����
// </c>

// <c1>Only using Msh Shell
//  <i>Only using Msh Shell
#define FINSH_USING_MSH_ONLY
// </c>

// <o>the priority of finsh thread <1-7>
//  <i>the priority of finsh thread
//  <i>Default: 6
#define __FINSH_THREAD_PRIORITY     1
#define FINSH_THREAD_PRIORITY       (RT_THREAD_PRIORITY_MAX / 8 * __FINSH_THREAD_PRIORITY + 1)

// <o>the stack of finsh thread <1-4096>
//  <i>the stack of finsh thread
//  <i>Default: 4096  (4096Byte)
#define FINSH_THREAD_STACK_SIZE 4096

// <o>the history lines of finsh thread <1-32>
//  <i>the history lines of finsh thread
//  <i>Default: 5
#define FINSH_HISTORY_LINES	        5

// <c1>Using symbol table in finsh shell
//  <i>Using symbol table in finsh shell
//#define FINSH_USING_SYMTAB
// </c>
// </h>

#endif //RTE_USING_FINSH

// <<< end of configuration section >>>

#endif

