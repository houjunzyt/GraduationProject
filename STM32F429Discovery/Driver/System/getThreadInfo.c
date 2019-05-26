#include "getthreadinfo.h"

/****************************************************************
*�������ܣ���ȡָ�������������׵�ַ
*�����������Ż������������������͡���Ϣ���顢��С
*����ֵ  ����
****************************************************************/
static void list_find_init(list_get_next_t *p, rt_uint8_t type, rt_list_t **array, int nr)
{
	struct rt_object_information *info;
	rt_list_t *list;

	info = rt_object_get_information((enum rt_object_class_type)type);//��ȡָ���������������͵������׵�ַ
	list = &(info->object_list);

	p->list = list;//��ֵ
	p->type = type;
	p->array = array;
	p->nr = nr;
	p->nr_out = 0;
}

/****************************************************************
*�������ܣ���ȡ��һ����������
*�����������ȡ��������������ʼ��ַ
*����ֵ  ��RT_NULL��ʧ��  �������ڵ��ַ
****************************************************************/
static rt_list_t *list_get_next(rt_list_t *current, list_get_next_t *arg)
{
    int first_flag = 0;
    rt_ubase_t level;
    rt_list_t *node, *list;
    rt_list_t **array;
    int nr;

    arg->nr_out = 0;

    if (!arg->nr)//�������Ȳ���Ϊ��
    {
      return (rt_list_t *)RT_NULL;
    }

    list = arg->list;//��ȡ��һ���ڵ�

    if (!current) //��һ�β���
    {
        node = list;
        first_flag = 1;
    }
    else//���ǵ�һ�β���
    {
        node = current;
    }

    level = rt_hw_interrupt_disable();

    if (!first_flag)
    {
        struct rt_object *obj;
        obj = rt_list_entry(node, struct rt_object, list);//node�Ƿ����
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
			node = node->next;//��һ������������node

			if (node == list)//���������
			{
					node = (rt_list_t *)RT_NULL;
					break;
			}
			nr++;//������һ
			*array++ = node;
			if (nr == arg->nr)//�������
			{
					break;
			}
    }
    rt_hw_interrupt_enable(level);
    arg->nr_out = nr;
    return node;
}

/****************************************************************
*�������ܣ���ȡ��һ����������
*���������
*����ֵ  ��
****************************************************************/
void list_thread(RT_Thread_Info *user_thread )
{
	rt_ubase_t level;
	list_get_next_t find_arg;
	rt_list_t *obj_list[LIST_FIND_OBJ_NR];//ָ������
	rt_list_t *next = (rt_list_t*)RT_NULL;
	//��ȡ���������Ĵ�С���׵�ַ
	list_find_init(&find_arg, RT_Object_Class_Thread, obj_list, sizeof(obj_list)/sizeof(obj_list[0]));
	do
	{
		next = list_get_next(next, &find_arg);//��ȡ�����̵߳���Ϣ
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
			user_thread[i].current_priority=thread->current_priority;//װ����Ϣ
			memcpy(user_thread[i].name,thread->name, sizeof (user_thread[i].name));
			user_thread[i].stat=thread->stat & RT_THREAD_STAT_MASK;
			user_thread[i].num=find_arg.nr_out;
		}
	}
	while (next != (rt_list_t*)RT_NULL);
}

