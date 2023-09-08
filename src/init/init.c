#include <kernel.h>

#include <drivers/DriverManager.h>
#include <drivers/Memory/Heap.h>
#include <drivers/Memory/Memory.h>
#include <drivers/Memory/PFA.h>
#include <drivers/Memory/scubadeeznutz.h>
#include <drivers/acpi_tables/acpi.h>
#include <drivers/hid/keyboard.h>
#include <drivers/io/pci.h>
#include <drivers/misc/time.h>


#include <utils/logging/logger.h>

extern struct KernelData data;

void init_HW()
{
    // Gather Data
    get_cpu_capabilities(&data.cpu_info);

    // Init the CPU
    sys_init_fpu();

    // Get Time
    time_get(&data.time);

    // Initialize screen and logger
    InitScreen(data.framebuffer);
    logger_set_output(LOGGER_OUTPUT_DEBUG);

    // Initialize GDT
    load_default_gdt();

    // initialize interrupts
    initialize_interrupts();
    init_falut_handler();

    // Init Memory stuff
    mem_init(data.memmap_resp);
    initPFA(data.memmap_resp);
    InitHeap(0x20000);
    // still no workie
    // TODO: fix scuba
    // scuba_init(data.memmap_resp, data.kernel_addr_resp);

    // Init the drivers
    rtc_init();
    init_drivers();
    // ACPI can wait 
    // TODO: finish ACPI
    //init_acpi(data.rsdp);

    enable_interrupts();
    log_info("Kernel Initialized Successfully");
}

void init_sched()
{
    
}

void init_sys()
{
    
}
