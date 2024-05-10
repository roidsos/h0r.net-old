#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <types/stdtypes.h>
#include <arch/x86_64/interrupts/interrupts.h>

typedef struct {
    bool present;
    char* name;
    Registers regs;
    uint8_t signal;
} process_t;

void sched_init();

uint32_t sched_add_process(char* name, void (*entry)(void));
void sched_kill(uint32_t pid);
#endif
