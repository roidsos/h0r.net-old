#ifndef KERNEL_H
#define KERNEL_H

#include "arch/x86/cpu.h"

#include "limine.h"
#include <types/stdtypes.h>
#include "flanterm.h"

// Struct to hold globals
struct KernelData {
    //System info
    void *efi_system_table_address;
    bool is_uefi_mode;
    CPUInfo cpu_info;

    //UEFI shit
    void* rsdp;
    void* hhdm_addr;
    
    //kterm shit
    struct flanterm_context* ft_ctx;  
    struct limine_framebuffer *framebuffer;
    
    //limine requests
    struct limine_memmap_response *memmap_resp;
    struct limine_smp_response *smp_resp;
    struct limine_kernel_address_response *ka_resp;
};

extern struct KernelData data;
#endif
