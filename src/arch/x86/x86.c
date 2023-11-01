#include "arch/x86/gdt.h"
#include "utils/error-handling/falut-handler.h"
#include <arch/x86/interrupts/interrupts.h>
#include <core/kernel.h>
#include <core/logging/logger.h>
#include <drivers/io/serial.h>
uint64_t kernel_stack[8192];
void main();

void _start(void) {
    // no unhandled interrupts plzz
    disable_interrupts();
    // Initialize screen and logger
    serial_init();
    logger_set_output(LOGGER_OUTPUT_COM1);
    log_info("x86 Init Target reached: IO\n");

    // Gather Data
    get_cpu_capabilities(&data.cpu_info);
    sys_init_fpu();
    gdt_init((uint64_t *)kernel_stack);
    log_info("x86 Init Target reached: CPU\n");

    // initialize interrupts
    initialize_interrupts();
    init_falut_handler();
    log_info("x86 Init Target reached: Interrupts\n");

    main();

    disable_interrupts();
    log_CRITICAL(NULL, HN_ERR_UNIMPLEMENTED,
                 "Out of bounds execution(arch x86_start func)");
}