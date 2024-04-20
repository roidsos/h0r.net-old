#include "arch/x86/gdt.h"
#include <arch/x86/interrupts/interrupts.h>
#include <core/kernel.h>
#include <vendor/printf.h>
uint64_t kernel_stack[8192];
void main();

void _start(void) {
    // no unhandled interrupts plzz
    disable_interrupts();
    // Initialize screen and logger
    dprintf("x86 Init Target reached: IO\n");

    // Gather Data
    get_cpu_capabilities(&data.cpu_info);
    sys_init_fpu();
    gdt_init((uint64_t *)kernel_stack);
    dprintf("x86 Init Target reached: CPU\n");

    // initialize interrupts
    initialize_interrupts();
    dprintf("x86 Init Target reached: Interrupts\n");

    main();

    dprintf("KErnel main function returned!!!!\n");
    while (true);
}