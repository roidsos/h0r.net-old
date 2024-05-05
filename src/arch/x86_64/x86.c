#include "arch/x86_64/gdt.h"
#include <arch/x86_64/interrupts/interrupts.h>
#include <arch/x86_64/pager.h>
#include <core/kernel.h>
#include <core/memory.h>
#include <drivers/output/cereal.h>
#include <utils/error.h>
#include <utils/log.h>
#include <vendor/printf.h>

uint64_t kernel_stack[8192];
void kmain();

void _start(void) {
    // Initialize screen and interrupts
    cereal_init();
    disable_interrupts();
    log_nice("x86_64 Init Target reached: IO\n");

    // CPU deez nutz
    data.pml4 = (uint64_t)PHYS_TO_VIRT(vmm_get_pagetable());
    get_cpu_capabilities(&data.cpu_info);
    sys_init_fpu();
    gdt_init((uint64_t *)kernel_stack);
    log_nice("x86_64 Init Target reached: CPU\n");

    // initialize interrupts
    initialize_interrupts();
    log_nice("x86_64 Init Target reached: Interrupts\n");

    kmain();

    trigger_psod(HN_ERR_KERNEL_EXITED, "The kernel Exited", NULL);
    while (true)
        ;
}
