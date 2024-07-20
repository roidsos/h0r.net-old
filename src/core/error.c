#include "error.h"
#include <arch/x86_64/LAPIC.h>
#include <arch/x86_64/cpu.h>
#include <core/mm/mem.h>
#include <core/libk/stdlib.h>
#include <stdint.h>
#include <core/utils/log.h>
#include <vendor/printf.h>

// WARN: Arch specific code in core/utils/ + TODO: smh move to arch/ or make
// architecture neutral
char *errors[] = {
    "HN_ERR_HW_EXCEPTION", "HN_ERR_KERNEL_EXITED", "HN_ERR_NO_ACPI",
    "HN_ERR_NO_FB",        "HN_ERR_OUT_OF_MEM",    "HN_ERR_LAI_PANIC",
    "HN_ERR_NO_FS", "HN_ERR_DRIVER_ERROR",
};

#define BGCOL "\x1b[48;2;17;0;34m"
#define FGCOL "\x1b[38;2;170;119;0m"

void trigger_psod(int error_code, char *details, Registers *regs) {
    // stop the scheduler, TODO: make this architecture independent
    lapic_timer_stop();

    Registers newregs;
    if (regs == NULL) {
        __asm__ volatile("movq %%rax, %0\r\n" : "=r"(newregs.rax) :);
        __asm__ volatile("movq %%rbx, %0\r\n" : "=r"(newregs.rbx) :);
        __asm__ volatile("movq %%rcx, %0\r\n" : "=r"(newregs.rcx) :);
        __asm__ volatile("movq %%rdx, %0\r\n" : "=r"(newregs.rdx) :);
        __asm__ volatile("movq %%rdi, %0\r\n" : "=r"(newregs.rdi) :);
        __asm__ volatile("movq %%rsi, %0\r\n" : "=r"(newregs.rsi) :);
        __asm__ volatile("movq %%r8,  %0\r\n" : "=r"(newregs.r8) :);
        __asm__ volatile("movq %%r9,  %0\r\n" : "=r"(newregs.r9) :);
        __asm__ volatile("movq %%r10, %0\r\n" : "=r"(newregs.r10) :);
        __asm__ volatile("movq %%r11, %0\r\n" : "=r"(newregs.r11) :);
        __asm__ volatile("movq %%r12, %0\r\n" : "=r"(newregs.r12) :);
        __asm__ volatile("movq %%r13, %0\r\n" : "=r"(newregs.r13) :);
        __asm__ volatile("movq %%r14, %0\r\n" : "=r"(newregs.r14) :);
        __asm__ volatile("movq %%r15, %0\r\n" : "=r"(newregs.r15) :);
        __asm__ volatile("movq %%rsp, %0\r\n" : "=r"(newregs.rsp) :);
        __asm__ volatile("movq %%rbp, %0\r\n" : "=r"(newregs.rbp) :);
        __asm__ volatile("leaq 0x0(%%rip), %0\r\n" : "=r"(newregs.rip) :);
        regs = &newregs;
    }

    log_error("%s\n", errors[error_code]);

        // background
    printf(BGCOL "\x1b[2J");
    printf(FGCOL "FUCKING HALT!\n");

    // title,error type and details
    printf("%s\n\n", errors[error_code]);
    if (details) {
        log_error("%s\n", details);
        printf("%s\n\n\n", details);
    }
    u64 faultig_addr;
    __asm__ volatile("movq %%cr2, %0\r\n" : "=r"(faultig_addr) :);
    printf("cr2: 0x%016llx\n\n", faultig_addr);
    log_error("cr2: 0x%016llx\n\n", faultig_addr);

    // Reg Dump
    printf("RIP=0x%016llx ,RSP=0x%016llx ,CS =0x%016llx ,SS =0x%016llx\n",
           regs->rip, regs->rsp, regs->cs, regs->ss);
    printf("RAX=0x%016llx ,RBX=0x%016llx ,RCX=0x%016llx ,RDX=0x%016llx\n",
           regs->rax, regs->rbx, regs->rcx, regs->rdx);
    printf("RSI=0x%016llx ,RDI=0x%016llx ,RBP=0x%016llx ,R9 =0x%016llx\n",
           regs->rsi, regs->rdi, regs->rbp, regs->r9);
    printf("R10=0x%016llx ,R11=0x%016llx ,R12=0x%016llx ,R13=0x%016llx\n",
           regs->r10, regs->r11, regs->r12, regs->r13);
    printf("R14=0x%016llx ,R15=0x%016llx ,ERROR_CODE=0x%016llx\n\n", regs->r14,
           regs->r15, regs->error);

    // Reg Dump
    dprintf("RIP=0x%016llx ,RSP=0x%016llx ,CS =0x%016llx ,SS =0x%016llx\n",
            regs->rip, regs->rsp, regs->cs, regs->ss);
    dprintf("RAX=0x%016llx ,RBX=0x%016llx ,RCX=0x%016llx ,RDX=0x%016llx\n",
            regs->rax, regs->rbx, regs->rcx, regs->rdx);
    dprintf("RSI=0x%016llx ,RDI=0x%016llx ,RBP=0x%016llx ,R9 =0x%016llx\n",
            regs->rsi, regs->rdi, regs->rbp, regs->r9);
    dprintf("R10=0x%016llx ,R11=0x%016llx ,R12=0x%016llx ,R13=0x%016llx\n",
            regs->r10, regs->r11, regs->r12, regs->r13);
    dprintf("R14=0x%016llx ,R15=0x%016llx ,ERROR_CODE=0x%016llx\n\n", regs->r14,
            regs->r15, regs->error);

    if (regs->rbp < (u64)hn_data.hhdm_off) {
        printf("NO backtrace sowwy(bad RBP)\n");
        dprintf("NO backtrace sowwy(bad RBP)\n");
        hcf();
    }

    printf("Backtrace:\n");
    dprintf("Backtrace:\n");
    struct stackframe64_t *frame = (struct stackframe64_t *)regs->rbp;
    while (true) {
        if (frame->RIP <= (u64)hn_data.hhdm_off) {
            break;
        }
        printf("    [%.16lx]  <No SymbolTable LMAO>", frame->RIP);
        dprintf("    [%.16lx]  <No SymbolTable LMAO>", frame->RIP);
        frame = (struct stackframe64_t *)frame->RBP;
    }; // TODO: support different memory models
    hcf();
}
