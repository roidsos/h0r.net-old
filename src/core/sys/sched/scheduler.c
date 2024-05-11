#include "scheduler.h"
#include <config.h>
#include <drivers/LAPIC.h>
#include <vendor/printf.h>
#include <klibc/string.h>
#include <core/mm/heap.h>
#include <vendor/printf.h>

process_t processes[MAX_PROCESSES];
uint32_t num_processes = 0;
uint32_t current_process = 0;
bool sched_running = false;

void schedule(Registers *regs) {
    if (!sched_running) {
        sched_running = true;
    }else{
        memcpy(&processes[current_process].regs, regs, sizeof(Registers));
    }
    next_process:
    current_process++;
    if (current_process > num_processes) current_process = 0;
    while(!(processes[current_process].state_flags == SCHED_STATE_READY)) {
        if (processes[current_process].state_flags == SCHED_STATE_DEAD) {
            memset(&processes[current_process], 0, sizeof(processes[current_process]));
            goto next_process;
        }
        current_process++;
        if (current_process > num_processes) current_process = 0;
    }

    memcpy(regs, &processes[current_process].regs, sizeof(Registers));

    lapic_timer_oneshot(1, 32);
    lapic_eoi();
}
void sched_init() {
    memset(processes, 0, sizeof(processes));
    register_ISR(32, schedule);
}

uint32_t sched_add_process(char* name, void (*entry)(void)){
    if (num_processes < MAX_PROCESSES) {
        processes[num_processes].state_flags = SCHED_STATE_READY;
        processes[num_processes].name = name;
        processes[num_processes].regs.rip = (uint64_t)entry;
        processes[num_processes].regs.rsp = (uint64_t)malloc(0x1000);
        processes[num_processes].regs.rbp = processes[num_processes].regs.rsp;
        processes[num_processes].regs.cs = 0x8;
        processes[num_processes].regs.ss = 0x10;
        processes[num_processes].regs.rflags = 0x202;
        num_processes++;
        return num_processes - 1;
    }
    return -1;
}
void sched_kill(uint32_t pid){
    if (pid < MAX_PROCESSES) {
        processes[pid].state_flags = SCHED_STATE_DEAD;
    }
}