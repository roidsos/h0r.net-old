#include <arch/x86/interrupts/interrupts.h>
#include <core/logging/logger.h>
#include <core/kernel.h>
void syscall_handler(__attribute__((unused)) Registers *regs) {
    switch (regs->rax)
    {
    case 0:
        break;//TODO: exit
    case 1://printchar
        flanterm_write(data.ft_ctx,(const char*)&regs->rbx,1);
        break;
    
    default:
        break;
    }
    EOI(0x80);
}
void init_syscall() { register_ISR(0x80, syscall_handler); }
