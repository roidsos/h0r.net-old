#include <kernel.h>

#include <drivers/DriverManager.h>
#include <drivers/Memory/Heap.h>
#include <drivers/Memory/Memory.h>
#include <drivers/Memory/PFA.h>
#include <drivers/Memory/scubadeeznutz.h>

#include "arch/x86_64/GDT/gdt.h"
#include <arch/x86_64/interrupts/interrupts.h>

#include "utils/error-handling/falut-handler.h"
#include "utils/screen.h"
#include <arch/x86_64/PIT.h>
#include <utils/logging/logger.h>

extern struct KernelData data;

void init_HW() {
    // Initialize screen and logger
    InitScreen(data.framebuffer);
    logger_set_output(LOGGER_OUTPUT_DEBUG);
    log_info("Init Target Reached: IO");

    // Gather Data
    get_cpu_capabilities(&data.cpu_info);
    sys_init_fpu();
    log_info("Init Target Reached: CPU");

    // Initialize GDT
    load_default_gdt();
    log_info("Init Target Reached: GDT");

    // initialize interrupts
    initialize_interrupts();
    init_falut_handler();
    log_info("Init Target Reached: Interrupts");

    // Init Memory stuff
    mem_init(data.memmap_resp);
    initPFA(data.memmap_resp);
    InitHeap(0x20000);
    // still no workie
    // TODO: fix scuba
    // scuba_init(data.memmap_resp, data.kernel_addr_resp);
    log_info("Init Target Reached: Memory");

    // Init x86 PC specific drivers
    pit_init();
    time_get(&data.time);
    log_info("Init Target Reached: x86 PC drivers");

    // Init the HW drivers
    init_drivers();
    log_info("Init Target Reached: Misc Drivers");
    
    enable_interrupts();
    log_info("Kernel Initialized Successfully");
}

void init_sched() {}

void init_sys() {}
