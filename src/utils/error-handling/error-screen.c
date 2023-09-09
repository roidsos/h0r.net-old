#include "error-screen.h"
#include <arch/x86_64/essential.h>
#include <vbe/font_renderer.h>
#include <vendor/printf.h>
void trigger_error(int code, Registers *regs, char *details) {
    ClearScreen(0x00FFFF00);
    centeredStringRenderer("FUCKING HALT!", 0, 2, 0x00AA0000);
    centeredStringRenderer(Hornet_error_codes[code], 30, 2, 0x00AA0000);
    centeredStringRenderer(details, 60, 2, 0x00AA0000);

    char resolve[256];
    snprintf_(resolve, 256,
              "RIP=0x%016llx,RSP=0x%016llx,CS =0x%016llx,SS =0x%016llx",
              regs->rip, regs->rsp, regs->cs, regs->ss);
    basicStringRender(resolve, 0, 120, 1, 0x00AA0000);
    log_info(resolve);

    snprintf_(resolve, 256,
              "RAX=0x%016llx,RBX=0x%016llx,RCX=0x%016llx,RDX=0x%016llx",
              regs->rax, regs->rbx, regs->rcx, regs->rdx);
    basicStringRender(resolve, 0, 135, 1, 0x00AA0000);
    log_info(resolve);

    snprintf_(resolve, 256,
              "RSI=0x%016llx,RDI=0x%016llx,RBP=0x%016llx,R9 =0x%016llx",
              regs->rsi, regs->rdi, regs->rbp, regs->r9);
    basicStringRender(resolve, 0, 150, 1, 0x00AA0000);
    log_info(resolve);

    snprintf_(resolve, 256,
              "R10=0x%016llx,R11=0x%016llx,R12=0x%016llx,R13=0x%016llx",
              regs->r10, regs->r11, regs->r12, regs->r13);
    basicStringRender(resolve, 0, 165, 1, 0x00AA0000);
    log_info(resolve);

    snprintf_(resolve, 256, "R14=0x%016llx,R15=0x%016llx,ERROR_CODE=0x%016llx",
              regs->r14, regs->r15, regs->error);
    basicStringRender(resolve, 0, 180, 1, 0x00AA0000);
    log_info(resolve);

    hcf();
}
