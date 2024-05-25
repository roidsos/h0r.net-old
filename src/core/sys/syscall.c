#include "syscall.h"
#include <core/sys/sched/sched.h>
#include <libk/macros.h>
#include <utils/log.h>
#include <core/sys/events.h>

u64 syscall(u64 number, u64 arg1, u64 arg2,UNUSED u64 arg3) {
    switch (number) {
    case 0: // sys_exit
        // TODO: exit codes
        sched_kill(sched_current_pid);
        break;
    case 1: // sys_event_create
        return event_create();
        break; // TODO
    case 2: // sys_event_destroy
        event_destroy(arg1);
        break; // TODO
    case 3: // sys_event_subscribe
        event_subscribe(sched_current_pid, arg1, (void*)arg2);
        break; // TODO
    case 4: // sys_event_unsubscribe
        event_unsubscribe(sched_current_pid, arg1);
        break; // TODO
    case 5: // sys_event_fire
        event_fire(arg1, (void*)arg2);
        break; // TODO 
    case 6: // sys_event_end
        return event_exit();
        break; // TODO
    default:
        log_error("Syscall %d not implemented", number);
    }
    return 0;
}
