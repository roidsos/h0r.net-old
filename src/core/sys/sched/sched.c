#include "sched.h"
#include <config.h>
#include <core/mm/heap.h>
#include <vendor/printf.h>
#include <libk/binary.h>

process_t processes[MAX_PROCESSES] = {0};
u32 sched_num_procs = 0;
u32 sched_current_pid = 0;
_bool sched_running = false;

// TODO: kernel and user processes
u32 sched_add_process(char *name, Registers init_state, u64 pagemap) {
    if (sched_num_procs < MAX_PROCESSES) {
        processes[sched_num_procs].name = name;
        processes[sched_num_procs].regs = init_state;
        processes[sched_num_procs].pagemap = pagemap;
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
        // WARN: Arch specific code in core/ + TODO: smh move to arch/ or make
        // architecture neutral
        // TODO: smh stop the timer to avoid double advance
        __asm__ volatile("int $0x20");
    }
}

process_t *sched_get_curr_process() { return &processes[sched_current_pid]; }

void sched_save_state(u32 pid)
{
    processes[pid].saved_regs = processes[pid].regs;
}

void sched_restore_state(u32 pid)
{
    processes[pid].regs = processes[pid].saved_regs;
    FLAG_SET(processes[pid].state_flags, SCHED_FLAGS_CHANGED);
    FLAG_UNSET(processes[sched_current_pid].state_flags, SCHED_FLAGS_IN_SYSCALL);
    if(pid == sched_current_pid){
        // TODO: smh stop the timer to avoid double advance
        __asm__ volatile("int $0x20");
    }
}

void sched_block(u32 pid) { processes[pid].state_flags = SCHED_STATE_BLOCKED; }

void sched_unblock(u32 pid) { processes[pid].state_flags = SCHED_STATE_READY; }