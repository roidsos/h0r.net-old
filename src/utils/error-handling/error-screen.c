#include "error-screen.h"
#include <arch/x86_64/essential.h>
#include <arch/x86_64/interrupts/interrupts.h>
#include <utils/logging/logger.h>
#include <vbe/font_renderer.h>
#include <vendor/printf.h>

#define BGCOL 0x00220011
#define FGCOL 0x00AA7700
void trigger_error(int code, Registers *regs, char *details) {
    disable_interrupts();

    ClearScreen(BGCOL);

    // title,error type and details
    centeredStringRenderer("FUCKING HALT!", 0, 2, FGCOL);
    centeredStringRenderer(Hornet_error_codes[code], 30, 2, FGCOL);
    centeredStringRenderer(details, 60, 2, FGCOL);

    // Reg Dump
    char resolve[256];
    snprintf_(resolve, 256,
              "RIP=0x%016llx ,RSP=0x%016llx ,CS =0x%016llx ,SS =0x%016llx",
              regs->rip, regs->rsp, regs->cs, regs->ss);
    basicStringRender(resolve, 0, 120, 1, FGCOL);
    log_info(resolve);

    snprintf_(resolve, 256,
              "RAX=0x%016llx ,RBX=0x%016llx ,RCX=0x%016llx ,RDX=0x%016llx",
              regs->rax, regs->rbx, regs->rcx, regs->rdx);
    basicStringRender(resolve, 0, 135, 1, FGCOL);
    log_info(resolve);

    snprintf_(resolve, 256,
              "RSI=0x%016llx ,RDI=0x%016llx ,RBP=0x%016llx ,R9 =0x%016llx",
              regs->rsi, regs->rdi, regs->rbp, regs->r9);
    basicStringRender(resolve, 0, 150, 1, FGCOL);
    log_info(resolve);

    snprintf_(resolve, 256,
              "R10=0x%016llx ,R11=0x%016llx ,R12=0x%016llx ,R13=0x%016llx",
              regs->r10, regs->r11, regs->r12, regs->r13);
    basicStringRender(resolve, 0, 165, 1, FGCOL);
    log_info(resolve);

    snprintf_(resolve, 256,
              "R14=0x%016llx ,R15=0x%016llx ,ERROR_CODE=0x%016llx", regs->r14,
              regs->r15, regs->error);
    basicStringRender(resolve, 0, 180, 1, FGCOL);
    log_info(resolve);

    hcf();
}
