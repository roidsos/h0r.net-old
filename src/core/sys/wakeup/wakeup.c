#include "wakeup.h"
#include "core/mm/mem.h"
#include "core/sys/executive/krnlexec/krnlexec.h"
#include "core/sys/executive/ELF/elf.h"
#include "drivers/storage/AHCI.h"
#include "libk/macros.h"
#include <config.h>

#include <core/sys/resman/VFS.h>

#include <core/mm/heap.h>
#include <core/mm/pmm.h>

#include <vendor/printf.h>

#include <arch/x86_64/interrupts/interrupts.h>
#include <arch/general/interrupts.h>

#include <drivers/ACPI/MCFG.h>
#include <drivers/meta/PCI.h>

#include <core/error.h>
#include <utils/log.h>

#include <core/sys/wakeup/gaia.h>

#include <lai/core.h>

extern int ACPI_revision;
extern _bool tar_init();

void wakeup_init_hw() {
    log_info("h0r.net identifies as v%u.%u.%u \"%s\"\n", KERNEL_VER_MAJOR,
             KERNEL_VER_MINOR, KERNEL_VER_PATCH, KERNEL_VER_CODENAME);

    DRIVER_ASSERT(pmm_init());

    DRIVER_ASSERT(tar_init());

    DRIVER_ASSERT(mcfg_init());
    DRIVER_ASSERT(pci_init());

    DRIVER_ASSERT(ahci_init());

    // lai_set_acpi_revision(hn_data.ACPI_ver);
    // lai_create_namespace();

    log_nice("Hardware sucessfully initialized!\n");
}

void wakeup_do_mounts() {
    //if (vfs_num_drives == 0) {
    //    trigger_psod(HN_ERR_NO_FS, "No filesystem found", NULL);
    //}

    log_nice("Filesystem sucessfully initialized!\n");
}
void wakeup_startup() {
    //setup for userland
    u64* kstack = PHYS_TO_VIRT(request_pages(STACK_SIZE) + STACK_SIZE*PAGE_SIZE);
    u64* sched_stack = PHYS_TO_VIRT(request_pages(STACK_SIZE) + STACK_SIZE*PAGE_SIZE);
    set_important_stacks(kstack, sched_stack);



    exec_elf("bin/test", "test", true);

    // start Gaia: the userspace portion of Wakeup
    //execute("Gaia", gaia_main);

    log_nice("Userland sucessfully initialized!\n");

    // kickstart the sched
    // WARN: Arch specific code in core/ + TODO: smh move to arch/ or make
    // architecture neutral
    __asm__("int $32");
}