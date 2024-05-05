#include "arch/x86_64/gdt.h"
#include <arch/x86_64/interrupts/interrupts.h>
#include <arch/x86_64/pager.h>
#include <backends/fb.h>
#include <core/kernel.h>
#include <core/memory.h>
#include <drivers/output/cereal.h>
#include <klibc/string.h>
#include <utils/error.h>
#include <utils/log.h>
#include <utils/psf2.h>
#include <vendor/printf.h>

uint64_t kernel_stack[8192];

static volatile struct limine_module_request mod_request = {
    .id = LIMINE_MODULE_REQUEST, .revision = 0};

void load_limine_modules() {
    for (size_t i = 0; i < mod_request.response->module_count; i++) {
        struct limine_file *mod = mod_request.response->modules[i];
        if (strcmp(mod->path, "/boot/kfont.psf") == 0) {
            data.ut_ctx = init_uterus_with_psf2_font(mod, data.framebuffer);
        } else {
            log_warn("Unknown module \"%s\" found", mod->path);
        }
    }

    if (data.ut_ctx == NULL) {
        data.ut_ctx = uterus_fb_simple_init(
            data.framebuffer->address, data.framebuffer->width,
            data.framebuffer->height, data.framebuffer->pitch);
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
    data.framebuffer = framebuffer_request.response->framebuffers[0];
    data.hhdm_off = (void *)hhdm_request.response->offset;
    load_limine_modules();
}

void kmain();

void _start(void) {
    limine_initialize_globals();
    // Initialize screen and interrupts
    cereal_init();
    disable_interrupts();
    log_nice("x86_64 Init Target reached: IO\n");

    // CPU deez nutz
    data.pml4 = (uint64_t)PHYS_TO_VIRT(vmm_get_pagetable());
    get_cpu_capabilities(&data.cpu_info);
    sys_init_fpu();
    gdt_init((uint64_t *)kernel_stack);
    log_nice("x86_64 Init Target reached: CPU\n");

    // initialize interrupts
    initialize_interrupts();
    log_nice("x86_64 Init Target reached: Interrupts\n");

    kmain();

    trigger_psod(HN_ERR_KERNEL_EXITED, "The kernel Exited", NULL);
    while (true)
        ;
}
