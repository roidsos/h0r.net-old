#include "sched.h"
#include <config.h>
#include <core/mm/heap.h>
#include <vendor/printf.h>

process_t processes[MAX_PROCESSES] = {0};
u32 sched_num_procs = 0;
u32 sched_current_pid = 0;
_bool sched_running = false;

// TODO: kernel and user processes
u32 sched_add_process(char *name, void (*entry)(void)) {
    if (sched_num_procs < MAX_PROCESSES) {
        processes[sched_num_procs].name = name;
        processes[sched_num_procs].regs.rip = (u64)entry;
        processes[sched_num_procs].regs.rsp = (u64)malloc(0x1000);
        processes[sched_num_procs].regs.cs = 0x8;
        processes[sched_num_procs].regs.ss = 0x10;
        processes[sched_num_procs].regs.rflags = 0x202;
        processes[sched_num_procs].state_flags = SCHED_STATE_READY;
        sched_num_procs++;
        return sched_num_procs - 1;
    }
    return -1;
}
void sched_kill(u32 pid) {
    if (pid < MAX_PROCESSES) {
        processes[pid].state_flags = SCHED_STATE_DEAD;
    }
    if (sched_current_pid == pid) {
        //TODO: make this arch independent and smh stop the timer to avoid double advance
        __asm__ volatile("int 0x20");
    }
}

process_t *sched_get_curr_process() { return &processes[sched_current_pid]; }

void sched_block(u32 pid) { processes[pid].state_flags = SCHED_STATE_BLOCKED; }

void sched_unblock(u32 pid) { processes[pid].state_flags = SCHED_STATE_READY; }