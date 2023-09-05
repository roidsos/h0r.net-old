#include "kernel.h"

#include <utils/logging/logger.h>

#include <arch/x86_64/interrupts/interrupts.h>
#include "arch/x86_64/GDT/gdt.h"

#include <drivers/DriverManager.h>
#include <drivers/Memory/Memory.h>
#include <drivers/Memory/PFA.h>
#include <drivers/Memory/Heap.h>
#include <drivers/Memory/scubadeeznutz.h>
#include <drivers/misc/time.h>
#include <drivers/io/pci.h>
#include <drivers/hid/keyboard.h>

#include <interface/desh.h>

#include "utils/screen.h"
#include "utils/error-handling/falut-handler.h"

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
static volatile struct limine_kernel_address_request kernel_address_request = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0
};
static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
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

    // Fetch the memory map.
    _data->kernel_addr_resp = kernel_address_request.response;

    // Fetch the memory map.
    _data->hhdm_resp = hhdm_request.response;
}

//create the single instance of the struct
struct KernelData data;

void init_kernel(){
    // Gather Data 
    handle_limine_requests(&data);
    get_cpu_capabilities(&data.cpu_info);
    
    //Init the CPU
    sys_init_fpu();
    
    //Get Time    
    time_get(&data.time);    

    //Initialize screen and logger
    InitScreen(data.framebuffer);
    logger_set_output(LOGGER_OUTPUT_DEBUG);
    
    //initialize interrupts
    initialize_interrupts();
    init_falut_handler();

    //Init Memory stuff
    load_default_gdt();
    mem_init(data.memmap_resp);
    initPFA(data.memmap_resp);
    InitHeap(0x20000);
    scuba_init(data.memmap_resp,data.kernel_addr_resp,data.hhdm_resp);
    
    //Init the drivers
    rtc_init();
    init_drivers();

    
    enable_interrupts();
    log_info("Kernel Initialized Successfully");

}

void _start(void) {
    // no unhandled interrupts plzz
    disable_interrupts();

    init_kernel();    
    
    printf_("========System Info========\n");
    
    printf_("UEFI mode: %d\n",data.is_uefi_mode);
    if(data.is_uefi_mode)
        printf_("EFI System Table Address: 0x%p\n", data.efi_system_table_address);

    printf_("CPU Vendor ID: %s\n", data.cpu_info.vendor);
    printf_("CPU Family: %d\n", data.cpu_info.family);
    printf_("CPU Model: %d\n", data.cpu_info.model);
    printf_("CPU Stepping: %d\n", data.cpu_info.stepping);
    printf_("CPU Extended Family: %d\n", data.cpu_info.ext_family);
    printf_("CPU Extended Model: %d\n", data.cpu_info.ext_model);
    printf_("CPU Capabilities (EDX): 0x%.8X\n", data.cpu_info.features[0]);
    printf_("CPU Capabilities (ECX): 0x%.8X\n", data.cpu_info.features[1]);

    printf_("Framebuffer Address: 0x%p\n", data.framebuffer->address);
    printf_("Framebuffer Width: %lu, Height: %lu, BPP: %u\n",
           data.framebuffer->width, data.framebuffer->height, data.framebuffer->bpp);
    
   
    printf_("Date: %02d/%02d/%d\n", data.time.month, data.time.day_of_month, data.time.year);
    printf_("Time: %02d:%02d:%02d\n", data.time.hours, data.time.minutes, data.time.seconds);
    
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
    


    
    //for (size_t i = 0; i < 7; i++)
    //{
    //    void* addr = request_page();
    //    printf_("0x%x\n",(size_t)addr);
    //}
    
    //((char*)0)[0] = 0;

    DeshInit();

    while (true)
    {
        DeshUpdate();
    }
    
    hcf();
}
