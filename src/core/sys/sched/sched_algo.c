#include "config.h"
#include "process.h"
#include <libk/string.h>

extern process_t processes[MAX_PROCESSES];
extern u32 sched_num_procs;
extern u32 sched_current_pid;

process_t next_process() {
    sched_current_pid++;
    if (sched_current_pid > sched_num_procs)
        sched_current_pid = 0;
    while (!(SCHED_GET_STATE(processes[sched_current_pid].state_flags) ==
             SCHED_STATE_READY)) {
        // WARN: Arch specific code in core/ + TODO: smh move to arch/ or make
        // architecture neutral
        if (SCHED_GET_STATE(processes[sched_current_pid].state_flags) ==
            SCHED_STATE_DEAD) {
            memset(&processes[sched_current_pid], 0,
                   sizeof(processes[sched_current_pid]));
        }
        sched_current_pid++;
        if (sched_current_pid > sched_num_procs)
            sched_current_pid = 0;
    }

    return processes[sched_current_pid];
}