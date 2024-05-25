#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

u32 sched_add_process(char *name, Registers init_state, u64 pagemap);
void sched_kill(u32 pid);

process_t* sched_get_curr_process();

void sched_save_state(u32 pid);
void sched_restore_state(u32 pid);

void sched_block(u32 pid);
void sched_unblock(u32 pid);

extern _bool sched_running;
extern u32 sched_current_pid;
#endif
