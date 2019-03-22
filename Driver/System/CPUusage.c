#include "cpuusage.h"

#define CPU_USAGE_CALC_TICK    100
#define CPU_USAGE_LOOP        100

static rt_uint8_t  cpu_usage_major = 0, cpu_usage_minor= 0;
static rt_uint32_t total_count = 0;

static void cpu_usage_idle_hook(void)
{
	rt_tick_t tick;
	rt_uint32_t count;
	volatile rt_uint32_t loop;
	if (total_count == 0)//调用只运行一次，计算时间在没有调度时用时时间
	{
		rt_enter_critical();
		tick = rt_tick_get();
		while(rt_tick_get() - tick < CPU_USAGE_CALC_TICK)
		{
			total_count ++;
			loop = 0;

			while (loop < CPU_USAGE_LOOP) loop ++;
		}
		rt_exit_critical();
	}
	count = 0;
	//计算在有系统调度的时间
	tick = rt_tick_get();
	while (rt_tick_get() - tick < CPU_USAGE_CALC_TICK)
	{
		count ++;
		loop  = 0;
		while (loop < CPU_USAGE_LOOP) loop ++;
	}

	//计算出使用率，整数部分以及小数部分
	if (count < total_count)
	{
		count = total_count - count;
		cpu_usage_major = (count * 100) / total_count;
		cpu_usage_minor = ((count * 100) % total_count) * 100 / total_count;
	}
	else
	{
		total_count = count;

		/* CPU就一个空闲钩子 */
		cpu_usage_major = 0;
		cpu_usage_minor = 0;
	}
}

void cpu_usage_get(rt_uint8_t *major, rt_uint8_t *minor)
{
	RT_ASSERT(major != RT_NULL);
	RT_ASSERT(minor != RT_NULL);

	*major = cpu_usage_major;
	*minor = cpu_usage_minor;
}

void cpu_usage_init(void)
{
  /*添加idle线程钩子 */
  rt_thread_idle_sethook(cpu_usage_idle_hook);
}





