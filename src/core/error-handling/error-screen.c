#include "error-screen.h"
#include <arch/x86/interrupts/interrupts.h>
#include <core/kernel.h>
#include <core/logging/logger.h>
#include <klibc/memory.h>
#include <types/string.h>
#include <vendor/printf.h>

#define BGCOL "\x1b[48;2;17;0;34m"
#define FGCOL "\x1b[38;2;170;119;0m"

void trigger_error(int code, Registers *regs, char *details) {
    disable_interrupts();

    printf(BGCOL "\x1b[2J");

    // title,error type and details
    printf(FGCOL "FUCKING HALT!\n");
    printf("%s\n\n", Hornet_error_codes[code]);
    printf("%s\n\n\n", details);

    // Reg Dump
    printf("RIP=0x%016llx ,RSP=0x%016llx ,CS =0x%016llx ,SS =0x%016llx\n",
           regs->rip, regs->rsp, regs->cs, regs->ss);
    printf("RAX=0x%016llx ,RBX=0x%016llx ,RCX=0x%016llx ,RDX=0x%016llx\n",
           regs->rax, regs->rbx, regs->rcx, regs->rdx);
    printf("RSI=0x%016llx ,RDI=0x%016llx ,RBP=0x%016llx ,R9 =0x%016llx\n",
           regs->rsi, regs->rdi, regs->rbp, regs->r9);
    printf("R10=0x%016llx ,R11=0x%016llx ,R12=0x%016llx ,R13=0x%016llx\n",
           regs->r10, regs->r11, regs->r12, regs->r13);
    printf("R14=0x%016llx ,R15=0x%016llx ,ERROR_CODE=0x%016llx\n", regs->r14,
           regs->r15, regs->error);

    log_info("RIP=0x%016llx ,RSP=0x%016llx ,CS =0x%016llx ,SS =0x%016llx\n",
             regs->rip, regs->rsp, regs->cs, regs->ss);
    log_info("RAX=0x%016llx ,RBX=0x%016llx ,RCX=0x%016llx ,RDX=0x%016llx\n",
             regs->rax, regs->rbx, regs->rcx, regs->rdx);
    log_info("RSI=0x%016llx ,RDI=0x%016llx ,RBP=0x%016llx ,R9 =0x%016llx\n",
             regs->rsi, regs->rdi, regs->rbp, regs->r9);
    log_info("R10=0x%016llx ,R11=0x%016llx ,R12=0x%016llx ,R13=0x%016llx\n",
             regs->r10, regs->r11, regs->r12, regs->r13);
    log_info("R14=0x%016llx ,R15=0x%016llx ,ERROR_CODE=0x%016llx\n", regs->r14,
             regs->r15, regs->error);

    hcf();
}
