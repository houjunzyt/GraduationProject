#ifndef _GETTHREADINFO_H_
#define _GETTHREADINFO_H_

#include "board.h"
#include "rtthread.h"
#include "stm32f4xx.h"
#include <rthw.h>
#include <string.h>
#include <stdio.h>

#define LIST_FIND_OBJ_NR 8

typedef struct
{
    rt_list_t *list;
    rt_list_t **array;
    rt_uint8_t type;
    int nr;             
    int nr_out;        
} list_get_next_t;


typedef struct
{
	char        name[RT_NAME_MAX];   //线程名字
	rt_uint8_t  stat;                //线程状态
	rt_uint8_t  current_priority;    //线程优先级
	rt_uint8_t  num;                 //线程个数
} RT_Thread_Info;


void list_thread(RT_Thread_Info *user_thread );

#endif
