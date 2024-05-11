#include "scheduler.h"
#include "utils/log.h"
#include <config.h>
#include <drivers/LAPIC.h>
#include <vendor/printf.h>
#include <klibc/string.h>
#include <core/mm/heap.h>
#include <vendor/printf.h>

process_t processes[MAX_PROCESSES];
uint32_t sched_num_procs = 0;
uint32_t sched_current_pid = 0;
bool sched_running = false;

void schedule(Registers *regs) {
    if (!sched_running) {
        sched_running = true;
    }else{
        memcpy(&processes[sched_current_pid].regs, regs, sizeof(Registers));
    }
    sched_current_pid++;
    if (sched_current_pid > sched_num_procs) sched_current_pid = 0;
    while(!(processes[sched_current_pid].state_flags == SCHED_STATE_READY)) {
        if (processes[sched_current_pid].state_flags == SCHED_STATE_DEAD) {
            memset(&processes[sched_current_pid], 0, sizeof(processes[sched_current_pid]));
        }
        sched_current_pid++;
        if (sched_current_pid > sched_num_procs) sched_current_pid = 0;
    }

    memcpy(regs, &processes[sched_current_pid].regs, sizeof(Registers));

    lapic_timer_oneshot(1, 32);
    lapic_eoi();
}
void sched_init() {
    memset(processes, 0, sizeof(processes));
    register_ISR(32, schedule);
}

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