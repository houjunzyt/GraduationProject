#ifndef _CPUUSAGE_H_
#define _CPUUSAGE_H_

#include <rtthread.h>
#include <rthw.h>

#define CALCULATION_PERIOD    1000

void cpu_usage_init(void);
void cpu_usage_get(rt_uint8_t *major, rt_uint8_t *minor);
uint16_t rt_get_CPUusage(void);
#endif
