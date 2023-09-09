#include "logger.h"
#include <vendor/printf.h>

char current_output;

void logger_set_output(char ID) { current_output = ID; }

void log_error(char *format, ...) {
    va_list va;
    va_start(va, format);

    if (current_output == LOGGER_OUTPUT_VGA) {
        printf(LOGGER_PREFIX_ERR);
        vprintf(format, va);
        printf("\n");
    }
    if (current_output == LOGGER_OUTPUT_DEBUG) {
        dprintf(LOGGER_PREFIX_ERR);
        vdprintf(format, va);
        dprintf("\n");
    }
    va_end(va);
}
void log_info(char *format, ...) {
    va_list va;
    va_start(va, format);

    if (current_output == LOGGER_OUTPUT_VGA) {
        printf(LOGGER_PREFIX_INFO);
        vprintf(format, va);
        printf("\n");
    }
    if (current_output == LOGGER_OUTPUT_DEBUG) {
        dprintf(LOGGER_PREFIX_INFO);
        vdprintf(format, va);
        dprintf("\n");
    }
    va_end(va);
}
void log_warning(char *format, ...) {
    va_list va;
    va_start(va, format);

    if (current_output == LOGGER_OUTPUT_VGA) {
        printf(LOGGER_PREFIX_WRN);
        vprintf(format, va);
        printf("\n");
    }
    if (current_output == LOGGER_OUTPUT_DEBUG) {
        dprintf(LOGGER_PREFIX_WRN);
        vdprintf(format, va);
        dprintf("\n");
    }
    va_end(va);
}
// Note: log_CRITICAL causes KERNEL PANIC
void log_CRITICAL(Registers *regs, uint8_t error_code, char *format, ...) {
    Registers newregs;
    if (regs == NULL) {
        asm("movq %%rax, %0\r\n" : "=r"(newregs.rax) :);
        asm("movq %%rbx, %0\r\n" : "=r"(newregs.rbx) :);
        asm("movq %%rcx, %0\r\n" : "=r"(newregs.rcx) :);
        asm("movq %%rdx, %0\r\n" : "=r"(newregs.rdx) :);
        asm("movq %%rdi, %0\r\n" : "=r"(newregs.rdi) :);
        asm("movq %%rsi, %0\r\n" : "=r"(newregs.rsi) :);
        asm("movq %%r8,  %0\r\n" : "=r"(newregs.r8) :);
        asm("movq %%r9,  %0\r\n" : "=r"(newregs.r9) :);
        asm("movq %%r10, %0\r\n" : "=r"(newregs.r10) :);
        asm("movq %%r11, %0\r\n" : "=r"(newregs.r11) :);
        asm("movq %%r12, %0\r\n" : "=r"(newregs.r12) :);
        asm("movq %%r13, %0\r\n" : "=r"(newregs.r13) :);
        asm("movq %%r14, %0\r\n" : "=r"(newregs.r14) :);
        asm("movq %%r15, %0\r\n" : "=r"(newregs.r15) :);
        asm("movq %%rsp, %0\r\n" : "=r"(newregs.rsp) :);
        asm("movq %%rbp, %0\r\n" : "=r"(newregs.rbp) :);
        asm("leaq 0x0(%%rip), %0\r\n" : "=r"(newregs.rip) :);
        regs = &newregs;
    }

    va_list va;
    va_start(va, format);

    if (current_output == LOGGER_OUTPUT_VGA) {
        printf(LOGGER_PREFIX_CRITICAL);
        vprintf(format, va);
        printf("(%s)\n", Hornet_error_codes[error_code]);
    }
    if (current_output == LOGGER_OUTPUT_DEBUG) {
        dprintf(LOGGER_PREFIX_CRITICAL);
        vdprintf(format, va);
        dprintf("(%s)\n", Hornet_error_codes[error_code]);
    }
    char resolve[256];
    vsnprintf_(resolve, 256, format, va);

    va_end(va);
    trigger_error(error_code, regs, resolve);
}
