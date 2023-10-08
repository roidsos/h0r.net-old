#include "kernel.h"

#include <arch/x86/essential.h>
#include <drivers/Memory/PFA.h>
#include <init/init.h>
#include <logging/logger.h>
#include <types/string.h>
#include <vendor/printf.h>

// ===============Limine Requests======================
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0};
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST, .revision = 0};
static volatile struct limine_efi_system_table_request
    efi_system_table_request = {.id = LIMINE_EFI_SYSTEM_TABLE_REQUEST,
                                .revision = 0};
static volatile struct limine_rsdp_request rsdp_request = {
    .id = LIMINE_RSDP_REQUEST, .revision = 0};
static volatile struct limine_smp_request smp_request = {
    .id = LIMINE_SMP_REQUEST, .revision = 0};
static volatile struct limine_module_request mod_request = {
    .id = LIMINE_MODULE_REQUEST, .revision = 0};
static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST, .revision = 0};
static volatile struct limine_kernel_address_request ka_request = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST, .revision = 0};

struct KernelData data;
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

    data.rsdp = rsdp_request.response->address;

    data.hhdm_addr = (void *)hhdm_request.response->offset;

    data.smp_resp = smp_request.response;

    data.ka_resp = ka_request.response;
}
void load_limine_modules() {
    bool config_found = false;
    for (size_t i = 0; i < mod_request.response->module_count; i++) {
        struct limine_file *mod = mod_request.response->modules[i];
        if (strcmp(mod->path, "/boot/hornet.conf")) {
            config_found = true;
            load_config(mod);
        } else if (strcmp(mod->path, "/boot/initramfs.tar")) {
            load_initramfs(mod);
        } else {
            log_info("%s found", mod->path);
            log_info("%s contents:\n %s", mod->path, mod->address);
        }
    }
    if (!config_found) {
        log_CRITICAL(NULL, HN_ERR_INVALID_CONF, "No configuration file found");
    }
}

// ================= KERNEL MAIN ============================
void debug_kernel_info() {
    log_info("========Kernel's Execution Enviroment========\n");

    log_info("UEFI mode: %d", data.is_uefi_mode);
    if (data.is_uefi_mode)
        log_info("EFI System Table Address: 0x%p",
                 data.efi_system_table_address);

    log_info("Memmap entry count: %lu\n", data.memmap_resp->entry_count);
    for (size_t i = 0; i < data.memmap_resp->entry_count; i++) {
        log_info("-Memmap entry #%lu: Base: 0x%lx, Length: 0x%lx, Type:%s", i,
                 data.memmap_resp->entries[i]->base,
                 data.memmap_resp->entries[i]->length,
                 memmap_type_names[data.memmap_resp->entries[i]->type]);
    }

    log_info("CPU count: %u", smp_request.response->cpu_count);
    for (size_t i = 0; i < data.smp_resp->cpu_count; i++) {
        struct limine_smp_info *cpu = data.smp_resp->cpus[i];
        log_info("-Core #%u Extra Argument: %u", i, cpu->extra_argument);
        log_info("-Core #%u Goto Address: %u", i, cpu->goto_address);
        log_info("-Core #%u Lapic ID: %u", i, cpu->lapic_id);
        log_info("-Core #%u Processor ID: %u\n", i, cpu->processor_id);
    }
}

void _start(void) {
    // no unhandled interrupts plzz
    disable_interrupts();

    handle_limine_requests();
    init_HW();
    debug_kernel_info();
    load_limine_modules();

    init_sched();

    while (true) {
        __asm__ volatile("hlt");
    }
}
