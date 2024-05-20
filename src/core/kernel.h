#ifndef KERNEL_H
#define KERNEL_H

#include "arch/x86_64/cpu.h"

#include "limine.h"
#include <libk/stdtypes.h>
#include "uterus.h"


// Struct to hold globals
struct HN_data_block {
    //System info
    CPUInfo cpu_info;
    uint64_t pml4;

    //ACPI shit
    void* hhdm_off;
    uint32_t lapic_base;

    //kterm shit
    struct uterus_context* ut_ctx;
    struct limine_framebuffer *framebuffer;
    struct limine_file *initramfs;
};
extern struct HN_data_block data;
#endif
