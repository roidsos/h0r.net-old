#include <arch/x86/interrupts/interrupts.h>
#include <core/logging/logger.h>
#include <core/sched/sched.h>
#include <core/kernel.h>
extern uint64_t current_PID;
extern void *krnlcr3;

void syscall_handler( Registers *regs) {
    __asm__ volatile("mov %0, %%cr3\r\n" : : "a"(krnlcr3));
    switch (regs->rax)
    {
    case 0:
        kill_process(current_PID);
        sched_next_process(regs);
        break;
    case 1://printchar
        flanterm_write(data.ft_ctx,(const char*)&regs->rbx,1);
        break;
    
    default:
        break;
    }
    EOI(0x80);
}
void init_syscall() { register_ISR(0x80, syscall_handler); }
