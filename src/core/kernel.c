#include "kernel.h"
#include "config.h"
#include "scheduler.h"

#include <core/mm/heap.h>
#include <core/mm/pmm.h>

#include <vendor/printf.h>

#include <arch/x86_64/interrupts/interrupts.h>
#include <drivers/ACPI/MCFG.h>
#include <drivers/ACPI/RSDT.h>
#include <drivers/APIC.h>
#include <drivers/IOAPIC.h>
#include <drivers/input/PS2.h>

#include <lai/core.h>

#include <klibc/stdlib.h>

#include <utils/error.h>
#include <utils/log.h>

struct HN_data_block data;

extern int ACPI_revision;

void kmain() {
    log_info("h0r.net identifies as v%u.%u.%u \"%s\"\n", KERNEL_VER_MAJOR,
             KERNEL_VER_MINOR, KERNEL_VER_PATCH, KERNEL_VER_CODENAME);

    if (!locate_rsdt()) {
        trigger_psod(HN_ERR_NO_ACPI, "NO ACPI FOUND lmao", NULL);
    }
    if (mcfg_init()) {
        iterate_pci();
    }
    pmm_init();

    char *kernel_heap = request_pages(HEAP_SIZE_IN_PAGES);
    heap_init((uint64_t)kernel_heap, HEAP_SIZE_IN_PAGES * PAGE_SIZE);

    apic_init();
    ioapic_init();
    enable_interrupts();
    sched_init();
    ps2_init();

    log_nice("Hardware sucessfully initialized!\n");

    // lai_set_acpi_revision(ACPI_revision);
    // lai_create_namespace();
    // printf("LAI works???");

    // kickstart the "scheduler"
    //__asm__("int $32");
    hlt();
}
