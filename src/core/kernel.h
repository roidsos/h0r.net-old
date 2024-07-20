#ifndef KERNEL_H
#define KERNEL_H

#include "arch/x86_64/cpu.h"

#include "limine.h"
#include <core/libk/stdint.h>
#include <core/libk/stdbool.h>
#include "uterus.h"


// Struct to hold globals
struct HN_data {
    //System info
    CPUInfo cpu_info;

    //ACPI shit
    void* hhdm_off;
    u32 ACPI_ver;
    u32 lapic_base;

    //kterm shit
    //WARN: Arch specific code in core/ + TODO: smh move to arch/ or make architecture neutral
    struct uterus_context* ut_ctx;
    struct limine_framebuffer *framebuffer;
    struct limine_file *initramfs;
};
extern u64 hn_pagemap; // separate cuz assembly
extern struct HN_data hn_data;
#endif
