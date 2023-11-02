#ifndef __SCHED_H__
#define __SCHED_H__
#include <types/stdtypes.h>
#include <arch/x86/interrupts/interrupts.h>

uint64_t create_process(void (*process_main)(),uint64_t prog_size,uint8_t pl_and_flags,bool krnl_mode);
void kill_process(uint64_t ID);

void sched_enable();
void sched_init();
void sched_next_process(Registers *regs);
#endif // __SCHED_H__