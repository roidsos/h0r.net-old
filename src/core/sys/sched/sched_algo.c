#include "config.h"
#include "process.h"
#include <libk/string.h>

extern process_t processes[MAX_PROCESSES];
extern u32 sched_num_procs;
extern u32 sched_current_pid;

void next_process() {
    sched_current_pid++;
    if (sched_current_pid > sched_num_procs)
        sched_current_pid = 0;
    while (!(processes[sched_current_pid].state_flags == SCHED_STATE_READY)) {
        if (processes[sched_current_pid].state_flags == SCHED_STATE_DEAD) {
            memset(&processes[sched_current_pid], 0,
                   sizeof(processes[sched_current_pid]));
        }
        sched_current_pid++;
        if (sched_current_pid > sched_num_procs)
            sched_current_pid = 0;
    }
}