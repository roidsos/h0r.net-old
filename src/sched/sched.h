#ifndef __SCHED_H__
#define __SCHED_H__
#include <types/stdtypes.h>

uint64_t create_process(void (*process_main)());
void kill_process(uint64_t ID);

void sched_enable();
void sched_init();

#endif // __SCHED_H__