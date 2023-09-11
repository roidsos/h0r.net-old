#ifndef KERNEL_H
#define KERNEL_H

#include "arch/x86/cpu.h"
#include <arch/x86/rtc.h>

#include "limine.h"
#include <types/stdtypes.h>
#include <parsing/tar.h>

// Struct to hold globals
struct KernelData {
    void *efi_system_table_address;
    bool is_uefi_mode;
    rtc_time_point time;
    CPUInfo cpu_info;
    struct tar_contents initramfs;
    void* rsdp;

    struct limine_framebuffer *framebuffer;
    struct limine_memmap_response *memmap_resp;
    struct limine_hhdm_response *hhdm_resp;
    struct limine_kernel_address_response *kernel_addr_resp;
    struct limine_smp_response *smp_resp;
};

extern struct KernelData data;
#endif
