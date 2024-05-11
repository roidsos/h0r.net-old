#ifndef SCHEDULER_H
#define SCHEDULER_H

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
} process_t;

void sched_init();

uint32_t sched_add_process(char* name, void (*entry)(void));
void sched_kill(uint32_t pid);

extern bool sched_running;
extern uint32_t sched_current_pid;
#endif
