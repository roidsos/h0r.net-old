#include "kernel.h"

#include <utils/logging/logger.h>

#include "utils/screen.h"
#include "arch/x86_64/GDT/gdt.h"

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
void load_default_gdt(){
    struct GDTDescriptor gdtDescriptor;
    gdtDescriptor.Size = sizeof(struct GDT) - 1;
    gdtDescriptor.Offset = (uint64_t)&DefaultGDT;
    LoadGDT(&gdtDescriptor);
}

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

uint64_t CalculateTotalMemorySize(struct limine_memmap_response* memmap) {
    uint64_t totalSize = 0;
    
    for (size_t i = 0; i < memmap->entry_count; i++) {
	totalSize += memmap->entries[i]->length;
    }
    return totalSize;
}

void _start(void) {
    logger_set_output(LOGGER_OUTPUT_DEBUG);

    handle_limine_requests(&data);
    load_default_gdt();
    log_info("Logger Works ;)");

    InitScreen(data.framebuffer);
    printf_("========System Info========\n");
    printf_("UEFI mode: %d\n",data.is_uefi_mode);

    // Print information about the UEFI system table
    printf_("EFI System Table Address: 0x%p\n", data.efi_system_table_address);

    // Print information about the framebuffer
    printf_("Framebuffer Address: 0x%p\n", data.framebuffer->address);
    printf_("Framebuffer Width: %lu, Height: %lu, BPP: %u\n",
           data.framebuffer->width, data.framebuffer->height, data.framebuffer->bpp);
    printf_("Total system memory size: %llu bytes\n", CalculateTotalMemorySize(data.memmap_resp));
    // Print memory map details
    printf_("Memmap entry count: %lu\n\n", data.memmap_resp->entry_count);
    for (size_t i = 0; i < data.memmap_resp->entry_count; i++) {
        printf_("  -Memmap entry #%lu: Base: 0x%lx, Length: 0x%lx, Type: %u\n", i,
               data.memmap_resp->entries[i]->base,
               data.memmap_resp->entries[i]->length,
               data.memmap_resp->entries[i]->type);
    }
    
    hcf();
}
