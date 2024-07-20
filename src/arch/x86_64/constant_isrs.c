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
#include <core/libk/binary.h>
#include <core/libk/string.h>

extern process_t next_process();
extern process_t processes[MAX_PROCESSES];

process_t current_process = {0};

void schedule(Registers *regs) {
    lapic_timer_stop();

    //__asm__ volatile("mov %0, %%cr3" : : "r"((u64)hn_pagemap));

    if (current_process.stack_base != 0) {
        if(!FLAG_READ(processes[sched_current_pid].state_flags,
                   SCHED_FLAGS_CHANGED)){
            current_process.regs = *regs;
        }
    }

    // the scheduling algorithm is separated from the scheduler
    current_process = next_process();

    *regs = processes[sched_current_pid].regs;
    __asm__ volatile("mov %0, %%cr3"
                     :
                     : "r"(VIRT_TO_PHYS(current_process.pagemap)));


    wrmsr(GS_KERNEL_MSR, (u64)&current_process);

    lapic_eoi();
    lapic_timer_oneshot(5, 32); // 5ms timeslice
}

void syscall_handler(Registers *regs) {
    // save the registers so it can be restored after the syscall
    current_process.regs = *regs;

    regs->rax = syscall(regs->rax, regs->rdi, regs->rsi, regs->rdx,regs->r10,regs->r8,regs->r9);
}

void cisrs_register() {
    register_ISR(0, schedule);
}