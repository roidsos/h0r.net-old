#include "kernel.h"
#include <init/init.h>

#include "arch/x86_64/GDT/gdt.h"
#include <utils/logging/logger.h>
#include <arch/x86_64/interrupts/interrupts.h>

#include <interface/desh.h>

#include "utils/error-handling/falut-handler.h"
#include "utils/screen.h"

// ===============Limine Requests======================
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0};
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST, .revision = 0};
static volatile struct limine_efi_system_table_request
    efi_system_table_request = {.id = LIMINE_EFI_SYSTEM_TABLE_REQUEST,
                                .revision = 0};
static volatile struct limine_kernel_address_request kernel_address_request = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST, .revision = 0};
static volatile struct limine_rsdp_request rsdp_request = {
    .id = LIMINE_RSDP_REQUEST, .revision = 0};
static volatile struct limine_smp_request smp_request = {
    .id = LIMINE_SMP_REQUEST, .revision = 0
    };

void handle_limine_requests() {
    //// Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL ||
        framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    //// Ensure we got a memmap.
    if (framebuffer_request.response == NULL) {
        hcf();
    }

    // Check if we got an EFI system table
    if (efi_system_table_request.response != NULL) {
        data.efi_system_table_address =
            efi_system_table_request.response->address;
        data.is_uefi_mode = true; // We are in UEFI mode
    } else {
        data.is_uefi_mode = false; // Not in UEFI mode
    }

    // Fetch the first framebuffer.
    data.framebuffer = framebuffer_request.response->framebuffers[0];

    // Fetch the memory map.
    data.memmap_resp = memmap_request.response;

    // Fetch the memory map.
    data.kernel_addr_resp = kernel_address_request.response;

    data.rsdp = rsdp_request.response->address;s
}

// ================= KERNEL MAIN ============================
struct KernelData data;

void _start(void) {
    // no unhandled interrupts plzz
    disable_interrupts();

    handle_limine_requests();
    init_HW();

    printf_("========System Info========\n");

    printf_("UEFI mode: %d\n", data.is_uefi_mode);
    if (data.is_uefi_mode)
        printf_("EFI System Table Address: 0x%p\n",
                data.efi_system_table_address);

    printf_("CPU count: %u\n",smp_request.response->cpu_count);
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
            data.framebuffer->width, data.framebuffer->height,
            data.framebuffer->bpp);

    printf_("Date: %02d/%02d/%d\n", data.time.month, data.time.day_of_month,
            data.time.year);
    printf_("Time: %02d:%02d:%02d\n", data.time.hours, data.time.minutes,
            data.time.seconds);

    printf_("Total system memory: %llu bytes\n", get_total_RAM());
    printf_("Free system memory: %llu bytes\n", get_free_RAM());
    printf_("Used system memory: %llu bytes\n", get_used_RAM());
    printf_("Reserved system memory: %llu bytes\n", get_reserved_RAM());

    //printf_("Memmap entry count: %lu\n\n", data.memmap_resp->entry_count);
    //for (size_t i = 0; i < data.memmap_resp->entry_count; i++) {
    //    printf_("  -Memmap entry #%lu: Base: 0x%lx, Length: 0x%lx, Type: %s\n",
    //            i, data.memmap_resp->entries[i]->base,
    //            data.memmap_resp->entries[i]->length,
    //            memmap_type_names[data.memmap_resp->entries[i]->type]);
    //}

    //((char*)0)[0] = 0;

    DeshInit();

    while (true) {
        DeshUpdate();
    }

    hcf();
}
