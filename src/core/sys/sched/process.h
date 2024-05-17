#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <types/stdtypes.h>
#include <arch/x86_64/interrupts/interrupts.h>

enum process_state {
    SCHED_STATE_NONEXISTENT = 0,
    SCHED_STATE_READY = 1,
    SCHED_STATE_BLOCKED = 2,
    SCHED_STATE_DEAD = 3
};

typedef struct {
    uint8_t state_flags;
    char* name;
    Registers regs;
    uint32_t tty_id;
} process_t;

#endif // __PROCESS_H__