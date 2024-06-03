#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <arch/x86_64/interrupts/interrupts.h>
#include <libk/stdint.h>
#include <libk/stdbool.h>

enum process_state {
    SCHED_STATE_NONEXISTENT = 0,
    SCHED_STATE_READY = 1,
    SCHED_STATE_BLOCKED = 2,
    SCHED_STATE_DEAD = 3
};

// bits 0-3 are the state
#define SCHED_GET_STATE(x) ((x) & 0b1111)
#define SCHED_FLAGS_IN_SYSCALL 1 << 4
#define SCHED_FLAGS_CHANGED 1 << 5

typedef struct {
    u8 state_flags;
    char* name;
    //WARN: Arch specific code in core/ + TODO: smh move to arch/ or make architecture neutral
    Registers regs;
    Registers saved_regs;
    Registers syscall_regs;
    u64 pagemap;
} process_t;

#endif // __PROCESS_H__