#include "syscall.h"
#include "core/kernel.h"
#include <core/sys/events.h>
#include <core/sys/sched/sched.h>
#include <libk/macros.h>
#include <utils/log.h>
#include <vendor/uterus/uterus.h>

u64 syscall(u64 number, u64 arg1, u64 arg2, UNUSED u64 arg3) {
    switch (number) {
    case 0: // sys_exit
        // TODO: exit codes
        sched_kill(sched_current_pid);
        break;
    case 1:                                             // sys_dbg_print
        uterus_write(hn_data.ut_ctx, (char *)&arg1, 1); // this is just a test
        break;
    case 2: // sys_event_create
        return event_create();
        break;
    case 3: // sys_event_destroy
        event_destroy(arg1);
        break;
    case 4: // sys_event_subscribe
        event_subscribe(sched_current_pid, arg1, (void *)arg2);
        break;
    case 5: // sys_event_unsubscribe
        event_unsubscribe(sched_current_pid, arg1);
        break;
    case 6: // sys_event_fire
        event_fire(arg1, (void *)arg2);
        break;
    case 7: // sys_event_end
        event_exit();
        break;
    default:
        log_error("Syscall %d not implemented", number);
    }
    return 0;
}
