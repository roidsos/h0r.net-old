#include "kernel.h"
#include "config.h"

#include <backends/fb.h>
#include <utils/psf2.h>
#include <vendor/printf.h>

#include <arch/x86_64/interrupts/interrupts.h>
#include <drivers/ACPI/RSDT.h>
#include <drivers/ACPI/MADT.h>

#include <klibc/stdlib.h>
#include <utils/error.h>

#include <klibc/string.h>

// ===============Limine Requests======================
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0};
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST, .revision = 0};
static volatile struct limine_module_request mod_request = {
    .id = LIMINE_MODULE_REQUEST, .revision = 0};
static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST, .revision = 0};

struct HN_data_block data;

// ===============Initialization Functions======================

void load_limine_modules() {
    for (size_t i = 0; i < mod_request.response->module_count; i++) {
        struct limine_file *mod = mod_request.response->modules[i];
        if (strcmp(mod->path, "/boot/kfont.psf") == 0) {
            data.ft_ctx = init_flanterm_with_psf2_font(mod, data.framebuffer);
        } else {
            dprintf("Unknown module \"%s\" found", mod->path);
        }
    }

    if (data.ft_ctx == NULL) {
        data.ft_ctx = flanterm_fb_simple_init(
            data.framebuffer->address, data.framebuffer->width,
            data.framebuffer->height, data.framebuffer->pitch);
    }
}

void initialize_globals() {
    // Limine stuff
    if (framebuffer_request.response == NULL ||
        framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }
    data.framebuffer = framebuffer_request.response->framebuffers[0];
    data.memmap_resp = memmap_request.response;
    data.hhdm_off = (void *)hhdm_request.response->offset;
    load_limine_modules();
}

void main() {
    initialize_globals();

    dprintf("h0r.net identifies as v%u.%u.%u\n\n", KERNEL_VER_MAJOR,
            KERNEL_VER_MINOR, KERNEL_VER_PATCH);

    if (!locate_rsdt()) {
        trigger_psod(HN_ERR_NO_ACPI, "you FUCKING dinosaur",NULL);
    }
    init_madt();

    while (true) {
        __asm__ volatile("hlt");
    }
}
