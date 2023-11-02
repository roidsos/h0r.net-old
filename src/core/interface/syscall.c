#include <arch/x86/interrupts/interrupts.h>
#include <core/logging/logger.h>
void syscall_handler(__attribute__((unused)) Registers *regs) {
    log_info("Syscall LESSGOOOOOOOOO!");
    EOI(0x80);
}
void init_syscall() { register_ISR(0x80, syscall_handler); }
