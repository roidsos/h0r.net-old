#include "sched.h"
#include "utils/log.h"
#include <config.h>
#include <drivers/LAPIC.h>
#include <vendor/printf.h>
#include <libk/string.h>
#include <core/mm/heap.h>
#include <vendor/printf.h>

process_t processes[MAX_PROCESSES] = {0};
uint32_t sched_num_procs = 0;
uint32_t sched_current_pid = 0;
bool sched_running = false;

extern void next_process();

void schedule(Registers *regs) {
    if (!sched_running) {
        sched_running = true;
    }else{
        memcpy(&processes[sched_current_pid].regs, regs, sizeof(Registers));
    }

    //the scheduling algorithm is separated from the scheduler
    next_process();

    memcpy(regs, &processes[sched_current_pid].regs, sizeof(Registers));

    lapic_timer_oneshot(1, 32);
    lapic_eoi();
}
void sched_init() {
    register_ISR(32, schedule);
}

//TODO: kernel and user processes
uint32_t sched_add_process(char* name, void (*entry)(void)){
    if (sched_num_procs < MAX_PROCESSES) {
        processes[sched_num_procs].name = name;
        processes[sched_num_procs].regs.rip = (uint64_t)entry;
        processes[sched_num_procs].regs.rsp = (uint64_t)malloc(0x1000);
        processes[sched_num_procs].regs.cs = 0x8;
        processes[sched_num_procs].regs.ss = 0x10;
        processes[sched_num_procs].regs.rflags = 0x202;
        processes[sched_num_procs].state_flags = SCHED_STATE_READY;
        sched_num_procs++;
        return sched_num_procs - 1;
    }
    return -1;
}
void sched_kill(uint32_t pid){
    if (pid < MAX_PROCESSES) {
        processes[pid].state_flags = SCHED_STATE_DEAD;
    }
}

process_t* sched_get_curr_process()
{
    return &processes[sched_current_pid];
}

void sched_block(uint32_t pid)
{
    processes[pid].state_flags = SCHED_STATE_BLOCKED;
}

void sched_unblock(uint32_t pid)
{
    processes[pid].state_flags = SCHED_STATE_READY;
}