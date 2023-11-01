#include <arch/x86/interrupts/interrupts.h>
#include <core/logging/logger.h>
void GDT_fault_handler(Registers *regs) {
    log_CRITICAL(regs, HN_ERR_GDT_FAULT, "General Protection fault");
    EOI(0xD);
}
void fucking_halt_handler(Registers *regs) {

    uint64_t cr2 = 0;
    __asm__ volatile("movq %%cr2, %0\r\n" : "=r"(cr2) :);

    log_CRITICAL(regs, HN_ERR_GDT_FAULT,
                 "Paging fault(faluting address: 0x%016llx)", cr2);
    EOI(0xE);
}
void init_falut_handler() {
    register_ISR(0xD, GDT_fault_handler);
    register_ISR(0xE, fucking_halt_handler);
}
