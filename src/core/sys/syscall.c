#include "syscall.h"
#include <libk/macros.h>
#include <utils/log.h>

u64 syscall(u64 number,UNUSED u64 arg1,UNUSED u64 arg2,UNUSED u64 arg3)
{
    switch (number) {
        default:
            log_error("Syscall %d not implemented", number);
            return 0;
    }
}
