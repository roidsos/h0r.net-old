#include <utils/logging/logger.h>
#include <arch/x86_64/interrupts/interrupts.h>
void GDT_fault_handler(Registers *regs) {
    log_CRITICAL(regs,HN_ERR_GDT_FALUT, "General Protection fault");
    EOI(0xD);
}
void fucking_halt_handler(Registers *regs) {

    log_CRITICAL(regs,HN_ERR_GDT_FALUT, "Paging fault");
    EOI(0xE);
}
void init_falut_handler() {
    register_ISR(0xD, GDT_fault_handler);
    register_ISR(0xE, fucking_halt_handler);
}
