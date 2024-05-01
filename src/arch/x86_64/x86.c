#include "arch/x86_64/gdt.h"
#include <arch/x86_64/interrupts/interrupts.h>
#include <core/kernel.h>
#include <utils/error.h>
#include <vendor/printf.h>
uint64_t kernel_stack[8192];
void kmain();

void _start(void) {
    // no unhandled interrupts plzz
    disable_interrupts();
    // Initialize screen and logger
    dprintf("x86_64 Init Target reached: IO\n");

    // CPU deez nutz
    get_cpu_capabilities(&data.cpu_info);
    sys_init_fpu();
    gdt_init((uint64_t *)kernel_stack);
    dprintf("x86_64 Init Target reached: CPU\n");

    // initialize interrupts
    initialize_interrupts();
    dprintf("x86_64 Init Target reached: Interrupts\n");

    kmain();

    trigger_psod(HN_ERR_KERNEL_EXITED, "The kernel Exited",NULL);
    while (true)
        ;
}
