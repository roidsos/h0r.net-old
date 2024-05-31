#include "arch/general/paging.h"
#include "arch/x86_64/gdt.h"
#include "drivers/ACPI/SDT.h"
#include <arch/x86_64/IOAPIC.h>
#include <arch/x86_64/LAPIC.h>
#include <arch/x86_64/constant_isrs.h>
#include <arch/x86_64/interrupts/interrupts.h>
#include <arch/x86_64/io/PS2.h>
#include <arch/x86_64/io/cereal.h>
#include <arch/x86_64/pager.h>
#include <backends/fb.h>
#include <core/kernel.h>
#include <core/mm/mem.h>
#include <libk/string.h>
#include <core/error.h>
#include <utils/log.h>
#include <utils/psf2.h>
#include <vendor/printf.h>

u64 kernel_stack[8192];

static volatile struct limine_module_request mod_request = {
    .id = LIMINE_MODULE_REQUEST, .revision = 0};

void load_limine_modules() {
    for (usize i = 0; i < mod_request.response->module_count; i++) {
        struct limine_file *mod = mod_request.response->modules[i];
        if (strcmp(mod->path, "/boot/kfont.psf") == 0) {
            hn_data.ut_ctx =
                init_uterus_with_psf2_font(mod, hn_data.framebuffer);
        } else if (strcmp(mod->path, "/boot/initramfs.tar") == 0) {
            hn_data.initramfs = mod;
        } else {
            log_warn("Unknown module \"%s\" found", mod->path);
        }
    }

    if (hn_data.ut_ctx == NULL) {
        hn_data.ut_ctx = uterus_fb_simple_init(
            hn_data.framebuffer->address, hn_data.framebuffer->width,
            hn_data.framebuffer->height, hn_data.framebuffer->pitch);
    }
}

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0};
static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST, .revision = 0};

void limine_initialize_globals() {
    if (framebuffer_request.response == NULL ||
        framebuffer_request.response->framebuffer_count < 1) {
        trigger_psod(HN_ERR_NO_FB, "No framebuffer found.", NULL);
    }
    hn_data.framebuffer = framebuffer_request.response->framebuffers[0];
    hn_data.hhdm_off = (void *)hhdm_request.response->offset;
    load_limine_modules();
}

void kmain();

static volatile struct limine_rsdp_request rsdp_request = {
    .id = LIMINE_RSDP_REQUEST, .revision = 0};
void _start(void) {
    limine_initialize_globals();

    cereal_init();
    disable_interrupts();
    log_nice("x86_64 Init Target reached: IO\n");

    get_cpu_capabilities(&hn_data.cpu_info);
    sys_init_fpu();
    gdt_init((u64 *)kernel_stack);
    log_nice("x86_64 Init Target reached: CPU\n");

    if (!find_rsdt(rsdp_request.response->address)) {
        trigger_psod(HN_ERR_NO_ACPI, "NO ACPI FOUND lmao", NULL);
    }
    log_nice("x86_64 Init Target reached: ACPI\n");

    hn_data.pagemap = vmm_get_pagemap();
    initialize_interrupts();
    lapic_init();
    ioapic_init();
    enable_interrupts();
    log_nice("x86_64 Init Target reached: Interrupts\n");

    ps2_init();
    cisrs_register();
    log_nice("x86_64 Init Target reached: Misc\n");

    kmain();

    trigger_psod(HN_ERR_KERNEL_EXITED, "The kernel Exited", NULL);
    while (true)
        ;
}
