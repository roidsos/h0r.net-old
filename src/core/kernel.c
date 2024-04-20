#include "kernel.h"
#include "config.h"

#include <backends/fb.h>
#include <klibc/stdlib.h>
#include <types/string.h>
#include <utils/psf2.h>
#include <vendor/printf.h>
#include <arch/x86_64/pager.h>

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

struct HN_data_block data;

// ===============Initialization Functions======================

void load_limine_modules() {
    bool config_found = false;
    for (size_t i = 0; i < mod_request.response->module_count; i++) {
        struct limine_file *mod = mod_request.response->modules[i];
        if (strcmp(mod->path, "/boot/kfont.psf") == 0) {
            data.ft_ctx = init_flanterm_with_psf2_font(mod, data.framebuffer);
        } else {
            dprintf("Unknown module \"%s\" found", mod->path);
        }
    }
    if (!config_found) {
        dprintf("No config found\n");
    }

    if (data.ft_ctx == NULL) {
        data.ft_ctx = flanterm_fb_simple_init(
            data.framebuffer->address, data.framebuffer->width,
            data.framebuffer->height, data.framebuffer->pitch);
    }
}

void initialize_globals() {
    // Set kernel version
    data.kernel_ver_major = KERNEL_VER_MAJOR;
    data.kernel_ver_minor = KERNEL_VER_MINOR;
    data.kernel_ver_patch = KERNEL_VER_PATCH;

    // Limine stuff
    if (framebuffer_request.response == NULL ||
        framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }
    if (framebuffer_request.response == NULL) {
        hcf();
    }
    if (efi_system_table_request.response != NULL) {
        data.efi_system_table_address =
            efi_system_table_request.response->address;
        data.is_uefi_mode = true; // We are in UEFI mode
    } else {
        data.is_uefi_mode = false; // Not in UEFI mode
    }
    data.framebuffer = framebuffer_request.response->framebuffers[0];
    data.memmap_resp = memmap_request.response;
    data.hhdm_addr = (void *)hhdm_request.response->offset;
    data.smp_resp = smp_request.response;
    data.ka_resp = ka_request.response;
    load_limine_modules();
}

void main() {
    // Limine
    initialize_globals();

    printf("Hello, World! from h0r.net v%u.%u.%u\n", data.kernel_ver_major,
           data.kernel_ver_minor, data.kernel_ver_patch);

    uint64_t pml4 = get_pagetable();
    printf("Limine-given PML4: 0x%p\n", pml4);
    uint64_t first_page = *(uint64_t *)(pml4 + data.hhdm_addr);
    printf("\nFirst page: \n");
    printf("  -Full Value: 0x%p\n", first_page);
    printf("  -Present bit: %d\n", (first_page & BITMASK_L4_PRESENT) != 0);
    printf("  -Writable bit: %d\n", (first_page & BITMASK_L4_WRITABLE) != 0);
    printf("  -User accesible bit: %d\n", (first_page & BITMASK_L4_USER) != 0);
    printf("  -Write through bit: %d\n", (first_page & BITMASK_L4_WRITE_THROUGH) != 0);
    printf("  -Cache disable bit: %d\n", (first_page & BITMASK_L4_CACHE_DISABLE) != 0);
    printf("  -Page size bit: %d\n", (first_page & BITMASK_L4_PAGE_SIZE) != 0);
    printf("  -Address: 0x%p\n", (void *)(first_page & BITMASK_L4_ADDR));



    while (true) {
        __asm__ volatile("hlt");
    }
}