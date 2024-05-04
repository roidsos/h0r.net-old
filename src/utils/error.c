#include "error.h"
#include <klibc/stdlib.h>
#include <core/memory.h>
#include <vendor/printf.h>
#include <arch/x86_64/cpu.h>

char *errors[] = {
    "HN_ERR_DE",
    "HN_ERR_DB",
    "HN_ERR_NMI",
    "HN_ERR_BP",
    "HN_ERR_OF",
    "HN_ERR_BR",
    "HN_ERR_UD",
    "HN_ERR_NM",
    "HN_ERR_DF",
    "HN_ERR_DEPRICATED",
    "HN_ERR_TS",
    "HN_ERR_NP",
    "HN_ERR_SS",// the SS is coming for you LMAO
    "HN_ERR_GP",
    "HN_ERR_PF",
    "HN_ERR_RESERVED",
    "HN_ERR_MF",
    "HN_ERR_AC",
    "HN_ERR_MC",
    "HN_ERR_XM",
    "HN_ERR_VE",
    "HN_ERR_CP",
    "HN_ERR_RESERVED",
    "HN_ERR_RESERVED",
    "HN_ERR_RESERVED",
    "HN_ERR_RESERVED",
    "HN_ERR_RESERVED",
    "HN_ERR_RESERVED",
    "HN_ERR_HV",
    "HN_ERR_VC",
    "HN_ERR_SX",
    "HN_ERR_RESERVED",
    "HN_ERR_KERNEL_EXITED",
    "HN_ERR_NO_ACPI",
    "HN_ERR_NO_FB",
    "HN_ERR_OUT_OF_MEM",
    "HN_ERR_LAI_PANIC"
};

#define BGCOL "\x1b[48;2;17;0;34m"
#define FGCOL "\x1b[38;2;170;119;0m"

void trigger_psod(int error_code, char *details,Registers* regs) {
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

    dprintf("Error UwU : %s", errors[error_code]);

    // background
    printf(BGCOL "\x1b[2J");
    printf(FGCOL "FUCKING HALT!\n");

    // title,error type and details
    printf("%s\n\n", errors[error_code]);
    if (details)
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

    if(regs->rbp == 0){
        printf("Backtrace not available(no RBP)\n");
        dprintf("Backtrace not available(no RBP)\n");
        hcf();
    }

    printf("Backtrace: NO\n");
    //dprintf("Backtrace:\n");
    //struct stackframe64_t *frame = (struct stackframe64_t *)PHYS_TO_VIRT(regs->rbp);
    //while (frame->RIP >= (uint64_t)0xffffffff80000000)//TODO: support different memory models
    //{
    //    printf ("    [%.16lx]  <No SymbolTable LMAO>", frame->RIP);
    //    dprintf("    [%.16lx]  <No SymbolTable LMAO>", frame->RIP);
    //    frame = (struct stackframe64_t *)PHYS_TO_VIRT(frame->RBP);
    //}
    hcf();
}
