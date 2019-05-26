#include "getthreadinfo.h"

/****************************************************************
*函数功能：获取指定对象容器的首地址
*传入参数：存放缓冲区、对象容器类型、信息数组、大小
*返回值  ：无
****************************************************************/
static void list_find_init(list_get_next_t *p, rt_uint8_t type, rt_list_t **array, int nr)
{
	struct rt_object_information *info;
	rt_list_t *list;

	info = rt_object_get_information((enum rt_object_class_type)type);//获取指定的容器对象类型的容器首地址
	list = &(info->object_list);

	p->list = list;//赋值
	p->type = type;
	p->array = array;
	p->nr = nr;
	p->nr_out = 0;
}

/****************************************************************
*函数功能：获取下一个对象容器
*传入参数：获取的容器，容器起始地址
*返回值  ：RT_NULL：失败  其他：节点地址
****************************************************************/
static rt_list_t *list_get_next(rt_list_t *current, list_get_next_t *arg)
{
    int first_flag = 0;
    rt_ubase_t level;
    rt_list_t *node, *list;
    rt_list_t **array;
    int nr;

    arg->nr_out = 0;

    if (!arg->nr)//容器长度不能为空
    {
      return (rt_list_t *)RT_NULL;
    }

    list = arg->list;//获取第一个节点

    if (!current) //第一次查找
    {
        node = list;
        first_flag = 1;
    }
    else//不是第一次查找
    {
        node = current;
    }

    level = rt_hw_interrupt_disable();

    if (!first_flag)
    {
        struct rt_object *obj;
        obj = rt_list_entry(node, struct rt_object, list);//node是否存在
        if ((obj->type & ~RT_Object_Class_Static) != arg->type)
        {
            rt_hw_interrupt_enable(level);
            return (rt_list_t *)RT_NULL;
        }
    }
    nr = 0;
    array = arg->array;
    while (1)
    {
			node = node->next;//下一个对象容器给node

			if (node == list)//遍历完成了
			{
					node = (rt_list_t *)RT_NULL;
					break;
			}
			nr++;//个数加一
			*array++ = node;
			if (nr == arg->nr)//遍历完成
			{
					break;
			}
    }
    rt_hw_interrupt_enable(level);
    arg->nr_out = nr;
    return node;
}

/****************************************************************
*函数功能：获取下一个对象容器
*传入参数：
*返回值  ：
****************************************************************/
void list_thread(RT_Thread_Info *user_thread )
{
	rt_ubase_t level;
	list_get_next_t find_arg;
	rt_list_t *obj_list[LIST_FIND_OBJ_NR];//指针数组
	rt_list_t *next = (rt_list_t*)RT_NULL;
	//获取对象容器的大小和首地址
	list_find_init(&find_arg, RT_Object_Class_Thread, obj_list, sizeof(obj_list)/sizeof(obj_list[0]));
	do
	{
		next = list_get_next(next, &find_arg);//获取所有线程的信息
		int i;
		for (i = 0; i < find_arg.nr_out; i++)
		{
			struct rt_object *obj;
			struct rt_thread  *thread;
			obj = rt_list_entry(obj_list[i], struct rt_object, list);
			level = rt_hw_interrupt_disable();
			if ((obj->type & ~RT_Object_Class_Static) != find_arg.type)
			{
				rt_hw_interrupt_enable(level);
				continue;
			}
			rt_hw_interrupt_enable(level);
			thread = (struct rt_thread*)obj;
			user_thread[i].current_priority=thread->current_priority;//装载信息
			memcpy(user_thread[i].name,thread->name, sizeof (user_thread[i].name));
			user_thread[i].stat=thread->stat & RT_THREAD_STAT_MASK;
			user_thread[i].num=find_arg.nr_out;
		}
	}
	while (next != (rt_list_t*)RT_NULL);
}

