#include "error-screen.h"
#include <klibc/memory.h>
#include <arch/x86/interrupts/interrupts.h>
#include <font/font_renderer.h>
#include <logging/logger.h>
#include <vendor/printf.h>

#define BGCOL 0x00220011
#define FGCOL 0x00AA7700
void trigger_error(int code, Registers *regs, char *details) {
    disable_interrupts();

    fr_clear_screen(BGCOL);

    // title,error type and details
    fr_render_centered_string("FUCKING HALT!", 0, 2, FGCOL);
    fr_render_centered_string(Hornet_error_codes[code], 30, 2, FGCOL);
    fr_render_centered_string(details, 60, 2, FGCOL);

    // Reg Dump
    char resolve[256];
    snprintf(resolve, 256,
             "RIP=0x%016llx ,RSP=0x%016llx ,CS =0x%016llx ,SS =0x%016llx",
             regs->rip, regs->rsp, regs->cs, regs->ss);
    fr_render_string(resolve, 0, 120, 1, FGCOL);
    log_info(resolve);

    snprintf(resolve, 256,
             "RAX=0x%016llx ,RBX=0x%016llx ,RCX=0x%016llx ,RDX=0x%016llx",
             regs->rax, regs->rbx, regs->rcx, regs->rdx);
    fr_render_string(resolve, 0, 135, 1, FGCOL);
    log_info(resolve);

    snprintf(resolve, 256,
             "RSI=0x%016llx ,RDI=0x%016llx ,RBP=0x%016llx ,R9 =0x%016llx",
             regs->rsi, regs->rdi, regs->rbp, regs->r9);
    fr_render_string(resolve, 0, 150, 1, FGCOL);
    log_info(resolve);

    snprintf(resolve, 256,
             "R10=0x%016llx ,R11=0x%016llx ,R12=0x%016llx ,R13=0x%016llx",
             regs->r10, regs->r11, regs->r12, regs->r13);
    fr_render_string(resolve, 0, 165, 1, FGCOL);
    log_info(resolve);

    snprintf(resolve, 256, "R14=0x%016llx ,R15=0x%016llx ,ERROR_CODE=0x%016llx",
             regs->r14, regs->r15, regs->error);
    fr_render_string(resolve, 0, 180, 1, FGCOL);
    log_info(resolve);

    hcf();
}
