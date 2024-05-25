#include "constant_isrs.h"
#include "arch/x86_64/interrupts/ISR.h"
#include "core/mm/mem.h"
#include <arch/x86_64/LAPIC.h>
#include <arch/x86_64/interrupts/interrupts.h>
#include <config.h>
#include <core/sys/sched/sched.h>
#include <core/sys/syscall.h>
#include <libk/string.h>

extern void next_process();
extern process_t processes[MAX_PROCESSES];

void schedule(Registers *regs) {
    if (!sched_running) {
        sched_running = true;
    } else {
        memcpy(&processes[sched_current_pid].regs, regs, sizeof(Registers));
    }

    // the scheduling algorithm is separated from the scheduler
    next_process();

    memcpy(regs, &processes[sched_current_pid].regs, sizeof(Registers));

    // TODO: FIX smh
    //__asm__ volatile("mov %0, %%cr3"
    //                 :
    //                 : "r"(VIRT_TO_PHYS(processes[sched_current_pid].pagemap)));

    lapic_timer_oneshot(1, 32);
    lapic_eoi();
}

void syscall_handler(Registers *regs) {
    regs->rax = syscall(regs->rax, regs->rdi, regs->rsi, regs->rdx);
}

void cisrs_register() {
    register_ISR(32, schedule);
    register_ISR(0x80, syscall_handler);
}
