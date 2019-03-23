#include <rtthread.h>
#include  "GUI.h"

static rt_sem_t 	DispSem;  		
static rt_sem_t  	EventSem;  		
static rt_sem_t  	KeySem;  	
static  int        KeyPressed;
static  char       KeyIsInited;
/*
*********************************************************************************************************
*                                        TIMING FUNCTIONS
*
* Notes: Some timing dependent routines of uC/GUI require a GetTime and delay funtion. 
*        Default time unit (tick), normally is 1 ms.
*********************************************************************************************************
*/

int  GUI_X_GetTime (void) 
{
	return (int)rt_tick_get();  
}


void  GUI_X_Delay (int period) 
{
	rt_thread_delay(period);
}


/*
*********************************************************************************************************
*                                          GUI_X_ExecIdle()
*********************************************************************************************************
*/
void GUI_X_ExecIdle (void) 
{
   GUI_X_Delay(1);
}


/*
*********************************************************************************************************
*                                    MULTITASKING INTERFACE FUNCTIONS
*
* Note(1): 1) The following routines are required only if uC/GUI is used in a true multi task environment, 
*             which means you have more than one thread using the uC/GUI API.  In this case the #define 
*             GUI_OS 1   needs to be in GUIConf.h
*********************************************************************************************************
*/

void  GUI_X_InitOS (void)
{ 
	DispSem = rt_sem_create("DispSem", 1, RT_IPC_FLAG_PRIO);
	if(DispSem == RT_NULL)
		rt_kprintf("Create DispSem Failed!\r\n");
	
	EventSem = rt_sem_create("EventSem", 0, RT_IPC_FLAG_PRIO);
	if(EventSem == RT_NULL)
		rt_kprintf("Create EventSem Failed!\r\n");
}


void  GUI_X_Lock (void)
{ 
	/* 获取互斥量 */
	rt_sem_take(DispSem, RT_WAITING_FOREVER);
}


void  GUI_X_Unlock (void)
{ 
	/* 发送互斥量 */
	rt_sem_release(DispSem);	
}


U32  GUI_X_GetTaskId (void) 
{ 
	rt_thread_t id;
	
	/* 获取线程优先级 */
	id = rt_thread_self();
	return ((U32)(id->current_priority));	
}

/*
*********************************************************************************************************
*                                        GUI_X_WaitEvent()
*                                        GUI_X_SignalEvent()
*********************************************************************************************************
*/


void GUI_X_WaitEvent (void) 
{
	/* 获取GUI信号量 */
	rt_sem_take(EventSem, RT_WAITING_FOREVER);
}


void GUI_X_SignalEvent (void) 
{
	/* 发送GUI信号量 */
	rt_sem_release(EventSem);
}

/*
*********************************************************************************************************
*                                      KEYBOARD INTERFACE FUNCTIONS
*
* Purpose: The keyboard routines are required only by some widgets.
*          If widgets are not used, they may be eliminated.
*
* Note(s): If uC/OS-II is used, characters typed into the log window will be placed	in the keyboard buffer. 
*          This is a neat feature which allows you to operate your target system without having to use or 
*          even to have a keyboard connected to it. (useful for demos !)
*********************************************************************************************************
*/

static  void  CheckInit (void) 
{
	if(KeyIsInited == 0)
	{
		KeyIsInited = 1;
		GUI_X_Init();
	}
}


void GUI_X_Init (void) 
{
	/* 创建按键互斥量 */

	KeySem = rt_sem_create("KeySem", 0, RT_IPC_FLAG_PRIO);
	if(KeySem == RT_NULL)
		rt_kprintf("Create KeySem Failed!\r\n");
}


int  GUI_X_GetKey (void) 
{
	int r;
	
	r = KeyPressed;
	CheckInit();
	KeyPressed = 0;
	return (r);
}


int  GUI_X_WaitKey (void) 
{
	int r;
	
	CheckInit();
	if(KeyPressed == 0)
		rt_sem_take(KeySem, RT_WAITING_FOREVER);		
	r = KeyPressed;
	KeyPressed = 0;
	return (r);	   
}


void  GUI_X_StoreKey (int k) 
{
	KeyPressed = k;
	rt_sem_release(KeySem);
}
void GUI_X_Log     (const char *s) { GUI_USE_PARA(s); }
void GUI_X_Warn    (const char *s) { GUI_USE_PARA(s); }
void GUI_X_ErrorOut(const char *s) { GUI_USE_PARA(s); }
