#ifndef KERNEL_H
#define KERNEL_H

#include "arch/x86/cpu.h"
#include <arch/x86/rtc.h>

#include "limine.h"
#include <types/stdtypes.h>
#include <parsing/tar.h>
#include <parsing/ini.h>
#include "flanterm.h"

// Struct to hold globals
struct KernelData {
    //System info
    void *efi_system_table_address;
    bool is_uefi_mode;
    CPUInfo cpu_info;

    //TIME!
    rtc_time_point time;

    //UEFI shit
    void* rsdp;
    void* hhdm_addr;

    //config and initramfs
    struct parsed_ini config;
    struct tar_contents initramfs;
    struct limine_file *conffile;
    struct limine_file *initrdfile;
    
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
