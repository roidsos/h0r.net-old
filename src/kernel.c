#include "kernel.h"

#include <utils/logging/logger.h>

#include "arch/x86_64/GDT/gdt.h"
#include <drivers/Memory/Memory.h>
#include <drivers/Memory/PFA.h>
#include <drivers/misc/time.h>

#include "utils/screen.h"

// ===============Limine Requests======================
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};
static volatile struct limine_efi_system_table_request efi_system_table_request = {
    .id = LIMINE_EFI_SYSTEM_TABLE_REQUEST,
    .revision = 0
};


void handle_limine_requests(struct KernelData *_data) {
    //// Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    //// Ensure we got a memmap.
    if (framebuffer_request.response == NULL) {
        hcf();
    }

    // Check if we got an EFI system table
    if (efi_system_table_request.response != NULL) {
        _data->efi_system_table_address = efi_system_table_request.response->address;
        _data->is_uefi_mode = true; // We are in UEFI mode
    } else {
        _data->is_uefi_mode = false; // Not in UEFI mode
    }

    // Fetch the first framebuffer.
    _data->framebuffer = framebuffer_request.response->framebuffers[0];

    // Fetch the memory map.
    _data->memmap_resp = memmap_request.response;
}

//create the single instance of the struct
struct KernelData data;

void init_kernel(){
    // Gather Data 
    handle_limine_requests(&data);
    get_cpu_capabilities(&data.cpu_info);

    //Initialize screen and logger
    InitScreen(data.framebuffer);
    logger_set_output(LOGGER_OUTPUT_VGA);
    
    //Init Memory stuff
    load_default_gdt();
    initPFA(data.memmap_resp);
    
    //Init random drivers
    rtc_init();
    sys_init_fpu();

    
    log_info("Kernel Initialized Successfully");

}

void _start(void) {
    init_kernel();    

    printf_("========System Info========\n");
    
    printf_("UEFI mode: %d\n",data.is_uefi_mode);
    if(data.is_uefi_mode)
        printf_("EFI System Table Address: 0x%p\n", data.efi_system_table_address);

    i_time_t time;
    time_get(&time);    
    printf_("Date: %02d/%02d/%d\n", time.month, time.day_of_month, time.year);
    printf_("Time: %02d:%02d:%02d\n", time.hours, time.minutes, time.seconds);

    printf_("CPU Vendor ID: %s\n", data.cpu_info.vendor);
    printf_("CPU Family: %d\n", data.cpu_info.family);
    printf_("CPU Model: %d\n", data.cpu_info.model);
    printf_("CPU Stepping: %d\n", data.cpu_info.stepping);
    printf_("Extended Family: %d\n", data.cpu_info.ext_family);
    printf_("Extended Model: %d\n", data.cpu_info.ext_model);

    // Display CPU capabilities
    printf_("CPU Capabilities (EDX): 0x%.8X\n", data.cpu_info.features[0]);
    printf_("CPU Capabilities (ECX): 0x%.8X\n", data.cpu_info.features[1]);

    printf_("Framebuffer Address: 0x%p\n", data.framebuffer->address);
    printf_("Framebuffer Width: %lu, Height: %lu, BPP: %u\n",
           data.framebuffer->width, data.framebuffer->height, data.framebuffer->bpp);
    
    printf_("Total system memory: %llu bytes\n", get_total_RAM());
    printf_("Free system memory: %llu bytes\n", get_free_RAM());
    printf_("Used system memory: %llu bytes\n", get_used_RAM());
    printf_("Reserved system memory: %llu bytes\n", get_reserved_RAM());
    
    printf_("Memmap entry count: %lu\n\n", data.memmap_resp->entry_count);
    for (size_t i = 0; i < data.memmap_resp->entry_count; i++) {
        printf_("  -Memmap entry #%lu: Base: 0x%lx, Length: 0x%lx, Type: %s\n", i,
               data.memmap_resp->entries[i]->base,
               data.memmap_resp->entries[i]->length,
               memmap_type_names[data.memmap_resp->entries[i]->type]);
    }
    
    hcf();
}
