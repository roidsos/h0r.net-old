#ifndef KERNEL_H
#define KERNEL_H

#include "arch/x86_64/cpu.h"

#include "limine.h"
#include <libk/stdint.h>
#include <libk/stdbool.h>
#include "uterus.h"


// Struct to hold globals
struct HN_data_block {
    //System info
    CPUInfo cpu_info;
    u64 pml4;

    //ACPI shit
    void* hhdm_off;
    u32 ACPI_ver;
    u32 lapic_base;

    //kterm shit
    struct uterus_context* ut_ctx;
    struct limine_framebuffer *framebuffer;
    struct limine_file *initramfs;
};
extern struct HN_data_block data;
#endif
