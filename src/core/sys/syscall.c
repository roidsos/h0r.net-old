#include "syscall.h"
#include <core/sys/sched/sched.h>
#include <libk/macros.h>
#include <utils/log.h>

u64 syscall(u64 number, UNUSED u64 arg1, UNUSED u64 arg2, UNUSED u64 arg3) {
    switch (number) {
    case 0: // sys_exit
        // TODO: exit codes
        sched_kill(sched_current_pid);
        break;
    case 1: // sys_event_create
        break; // TODO
    case 2: // sys_event_destroy
        break; // TODO
    case 3: // sys_event_subscribe
        break; // TODO
    case 4: // sys_event_unsubscribe
        break; // TODO
    case 5: // sys_event_fire
        break; // TODO 
    case 6: // sys_event_end
        break; // TODO
    default:
        log_error("Syscall %d not implemented", number);
    }
    return 0;
}
