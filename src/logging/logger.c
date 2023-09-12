#include "logger.h"
#include <drivers/io/serial.h>
#include <vendor/printf.h>
uint16_t current_output;

void logger_set_output(uint16_t ID) { current_output = ID; }

void log_error(char *format, ...) {
    va_list va;
    va_start(va, format);

    if (current_output == LOGGER_OUTPUT_VGA) {
        printf(LOGGER_PREFIX_ERR);
        vprintf(format, va);
        printf("\n");
    } else if (current_output == LOGGER_OUTPUT_DEBUG) {
        dprintf(LOGGER_PREFIX_ERR);
        vdprintf(format, va);
        dprintf("\n");
    } else if (current_output >= 0x2E8 && current_output <= 0x5F8) {
        serial_write_string(LOGGER_PREFIX_ERR, current_output);

        char resolve[256];
        vsnprintf(resolve, 256, format, va);
        serial_write_string(resolve, current_output);

        serial_write_string("\n", current_output);
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
    } else if (current_output == LOGGER_OUTPUT_DEBUG) {
        dprintf(LOGGER_PREFIX_INFO);
        vdprintf(format, va);
        dprintf("\n");
    } else if (current_output >= 0x2E8 && current_output <= 0x5F8) {
        serial_write_string(LOGGER_PREFIX_INFO, current_output);

        char resolve[256];
        vsnprintf(resolve, 256, format, va);
        serial_write_string(resolve, current_output);

        serial_write_string("\n", current_output);
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
    } else if (current_output == LOGGER_OUTPUT_DEBUG) {
        dprintf(LOGGER_PREFIX_WRN);
        vdprintf(format, va);
        dprintf("\n");
    } else if (current_output >= 0x2E8 && current_output <= 0x5F8) {
        serial_write_string(LOGGER_PREFIX_WRN, current_output);

        char resolve[256];
        vsnprintf(resolve, 256, format, va);
        serial_write_string(resolve, current_output);

        serial_write_string("\n", current_output);
    }
    va_end(va);
}
// Note: log_CRITICAL causes KERNEL PANIC
void log_CRITICAL(Registers *regs, uint8_t error_code, char *format, ...) {
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

    va_list va;
    va_start(va, format);

    if (current_output == LOGGER_OUTPUT_VGA) {
        printf(LOGGER_PREFIX_CRITICAL);
        vprintf(format, va);
        printf("(%s)\n", Hornet_error_codes[error_code]);
    } else if (current_output == LOGGER_OUTPUT_DEBUG) {
        dprintf(LOGGER_PREFIX_CRITICAL);
        vdprintf(format, va);
        dprintf("(%s)\n", Hornet_error_codes[error_code]);
    } else if (current_output >= 0x2E8 && current_output <= 0x5F8) {
        serial_write_string(LOGGER_PREFIX_CRITICAL, current_output);

        char resolve[256];
        vsnprintf(resolve, 256, format, va);
        serial_write_string(resolve, current_output);

        serial_write_string("\n", current_output);
    }
    char resolve[256];
    vsnprintf(resolve, 256, format, va);

    va_end(va);
    trigger_error(error_code, regs, resolve);
}
