#include <arch/x86/interrupts/interrupts.h>
#include <core/logging/logger.h>
#include <core/sched/sched.h>
extern bool is_sched_active;
extern uint64_t current_PID;
void GDT_fault_handler(Registers *regs) {
    if (is_sched_active) {
        if (current_PID == 0) {
            log_CRITICAL(regs, HN_ERR_GDT_FAULT,
                         "General Protection fault in Init System");
        } else {
            log_error("General Protection fault in PID %u", current_PID);
            kill_process(current_PID);
            sched_next_process(regs);
        }
    } else {
        log_CRITICAL(regs, HN_ERR_GDT_FAULT,
                     "General Protection fault in kernel");
    }
    EOI(0xD);
}

void fucking_halt_handler(Registers *regs) {
    uint64_t cr2 = 0;
    __asm__ volatile("movq %%cr2, %0\r\n" : "=r"(cr2) :);
    if (is_sched_active) {
        if (current_PID == 0) {
            log_CRITICAL(
                regs, HN_ERR_GDT_FAULT,
                "Paging fault in Init System(faluting address: 0x%016llx)",
                cr2);
        } else {
            log_error("Paging fault(faluting address: 0x%016llx) in PID %u",
                      cr2, current_PID);
            kill_process(current_PID);
            sched_next_process(regs);
        }
    } else {
        log_CRITICAL(regs, HN_ERR_GDT_FAULT,
                     "Paging fault in kernel(faluting address: 0x%016llx)",
                     cr2);
    }
    EOI(0xE);
}
void init_falut_handler() {
    register_ISR(0xD, GDT_fault_handler);
    register_ISR(0xE, fucking_halt_handler);
}
