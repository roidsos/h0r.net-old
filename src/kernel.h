#ifndef KERNEL_H
#define KERNEL_H

#include "limine.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Struct to hold globals
struct KernelData {
    void *efi_system_table_address;
    bool is_uefi_mode;
    
    struct limine_framebuffer *framebuffer;
    struct limine_memmap_response *memmap_resp;
};

#endif
