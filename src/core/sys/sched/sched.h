#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

void sched_init();

uint32_t sched_add_process(char* name, void (*entry)(void));
void sched_kill(uint32_t pid);

process_t* sched_get_curr_process();

void sched_block(uint32_t pid);
void sched_unblock(uint32_t pid);

extern bool sched_running;
extern uint32_t sched_current_pid;
#endif
