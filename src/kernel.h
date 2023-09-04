#ifndef KERNEL_H
#define KERNEL_H

#include "arch/x86_64/cpu.h"
#include <drivers/misc/time.h>

#include "limine.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Struct to hold globals
struct KernelData {
    void *efi_system_table_address;
    bool is_uefi_mode;
    i_time_t time;
    CPUInfo cpu_info;

    struct limine_framebuffer *framebuffer;
    struct limine_memmap_response *memmap_resp;
    struct limine_hhdm_response *hhdm_resp;
    struct limine_kernel_address_response *kernel_addr_resp;
};

#endif
