#ifndef KERNEL_H
#define KERNEL_H

#include "arch/x86_64/cpu.h"

#include "limine.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Struct to hold globals
struct KernelData {
    void *efi_system_table_address;
    bool is_uefi_mode;
    CPUInfo cpu_info;

    struct limine_framebuffer *framebuffer;
    struct limine_memmap_response *memmap_resp;
};

#endif
