#include "core/kernel.h"

#include <backends/fb.h>
#include <core/Memory/PFA.h>
#include <core/init/init.h>
#include <core/logging/logger.h>
#include <klibc/memory.h>
#include <types/string.h>
#include <utils/psf2.h>
#include <vendor/printf.h>

// ===============Limine Requests======================
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0};
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST, .revision = 0};
static volatile struct limine_efi_system_table_request
    efi_system_table_request = {.id = LIMINE_EFI_SYSTEM_TABLE_REQUEST,
                                .revision = 0};
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

    // Misc shit
    data.framebuffer = framebuffer_request.response->framebuffers[0];

    data.memmap_resp = memmap_request.response;

    data.hhdm_addr = (void *)hhdm_request.response->offset;

    data.smp_resp = smp_request.response;

    data.ka_resp = ka_request.response;
}
void load_limine_modules() {
    bool config_found = false;
    for (size_t i = 0; i < mod_request.response->module_count; i++) {
        struct limine_file *mod = mod_request.response->modules[i];
        if (strcmp(mod->path, "/boot/hornet.conf") == 0) {
            config_found = true;
            data.conffile = mod;
        } else if (strcmp(mod->path, "/boot/initramfs.tar") == 0) {
            data.initrdfile = mod;
        } else if (strcmp(mod->path, "/boot/kfont.psf") == 0) {
            data.ft_ctx = init_flanterm_with_psf2_font(mod, data.framebuffer);
        } else {
            log_info("Unknown module \"%s\" found", mod->path);
        }
    }
    if (!config_found) {
        log_CRITICAL(NULL, HN_ERR_INVALID_CONF, "No configuration file found");
    }

    if (data.ft_ctx == NULL) {
        data.ft_ctx = flanterm_fb_simple_init(
            data.framebuffer->address, data.framebuffer->width,
            data.framebuffer->height, data.framebuffer->pitch);
    }
}
// ================= KERNEL MAIN ============================

void do_mounts();
void tar_init();
void main() {
    // Limine
    handle_limine_requests();
    load_limine_modules();

    // Hardware
    init_HW();

    // config and initramfs
    data.config = parse_ini(data.conffile->address);
    data.initramfs = parse_tar(data.initrdfile->address, data.initrdfile->size);

    // init the VFS and filesystem drivers
    tar_init();
    do_mounts();

    // start the init system
    initsys_start();

    while (true) {
        __asm__ volatile("hlt");
    }
}