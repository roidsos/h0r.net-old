#include "constant_isrs.h"
#include "arch/x86_64/cpu.h"
#include "arch/x86_64/interrupts/ISR.h"
#include "core/mm/mem.h"
#include "core/sys/sched/process.h"
#include <arch/x86_64/LAPIC.h>
#include <arch/x86_64/interrupts/interrupts.h>
#include <config.h>
#include <core/sys/sched/sched.h>
#include <core/sys/syscall.h>
#include <libk/binary.h>
#include <libk/string.h>

extern void next_process();
extern process_t processes[MAX_PROCESSES];

void schedule(Registers *regs) {
    if (!sched_running) {
        sched_running = true;
    } else {
        if (!FLAG_READ(processes[sched_current_pid].state_flags,
                       SCHED_FLAGS_CHANGED)) {
            if (FLAG_READ(processes[sched_current_pid].state_flags,
                          SCHED_FLAGS_IN_SYSCALL)) {
                memcpy(&processes[sched_current_pid].syscall_regs, regs,
                       sizeof(Registers));
            } else {
                memcpy(&processes[sched_current_pid].regs, regs,
                       sizeof(Registers));
            }
        }
    }

    // the scheduling algorithm is separated from the scheduler
    next_process();

    if (FLAG_READ(processes[sched_current_pid].state_flags,
                  SCHED_FLAGS_IN_SYSCALL)) {
        memcpy(regs, &processes[sched_current_pid].syscall_regs,
               sizeof(Registers));
    } else {
        memcpy(regs, &processes[sched_current_pid].regs, sizeof(Registers));
    }

    wrmsr(GS_KERNEL_MSR, (u64)&processes[sched_current_pid].stackstate);

    // setting the timer for next yield, have to do it in the kernel pagemap
    lapic_timer_oneshot(5, 32); // 5ms timeslice
    lapic_eoi();

    if (processes[sched_current_pid].state_flags & SCHED_FLAGS_IN_SYSCALL)
        return;

    __asm__ volatile("mov %0, %%cr3"
                     :
                     : "r"(VIRT_TO_PHYS(processes[sched_current_pid].pagemap)));
}

void syscall_handler(Registers *regs) {
    // save the registers so it can be restored after the syscall
    memcpy(&processes[sched_current_pid].regs, regs, sizeof(Registers));

    FLAG_SET(processes[sched_current_pid].state_flags, SCHED_FLAGS_IN_SYSCALL);
    regs->rax = syscall(regs->rax, regs->rdi, regs->rsi, regs->rdx,regs->r10,regs->r8,regs->r9);
    FLAG_UNSET(processes[sched_current_pid].state_flags,
               SCHED_FLAGS_IN_SYSCALL);
}

void cisrs_register() {
    register_ISR(32, schedule);
}
