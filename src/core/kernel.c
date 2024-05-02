#include "kernel.h"
#include "config.h"
#include "scheduler.h"

#include <backends/fb.h>
#include <utils/psf2.h>
#include <vendor/printf.h>

#include <arch/x86_64/interrupts/interrupts.h>
#include <drivers/ACPI/RSDT.h>
#include <drivers/ACPI/MCFG.h>
#include <drivers/audio/pcspk.h>
#include <drivers/output/cereal.h>
#include <drivers/input/PS2.h>
#include <drivers/APIC.h>
#include <drivers/IOAPIC.h>

#include <klibc/stdlib.h>
#include <klibc/string.h>

#include <utils/error.h>

struct HN_data_block data;

// ===============Initialization Functions======================
static volatile struct limine_module_request mod_request = {
    .id = LIMINE_MODULE_REQUEST, .revision = 0};

void load_limine_modules() {
    for (size_t i = 0; i < mod_request.response->module_count; i++) {
        struct limine_file *mod = mod_request.response->modules[i];
        if (strcmp(mod->path, "/boot/kfont.psf") == 0) {
            data.ut_ctx = init_uterus_with_psf2_font(mod, data.framebuffer);
        } else {
            dprintf("Unknown module \"%s\" found", mod->path);
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

void initialize_globals() {
    // Limine stuff
    if (framebuffer_request.response == NULL ||
        framebuffer_request.response->framebuffer_count < 1) {
        trigger_psod(HN_ERR_NO_FB,"No framebuffer found.",NULL);
    }
    data.framebuffer = framebuffer_request.response->framebuffers[0];
    data.hhdm_off = (void *)hhdm_request.response->offset;
    load_limine_modules();
}

void kmain() {
    initialize_globals();
    cereal_init();

    dprintf("h0r.net identifies as v%u.%u.%u\n\n", KERNEL_VER_MAJOR,
            KERNEL_VER_MINOR, KERNEL_VER_PATCH);

    if (!locate_rsdt()) {
        trigger_psod(HN_ERR_NO_ACPI, "you FUCKING dinosaur",NULL);
    }
    //if (init_mcfg()) {
    //    printf("PCI yayyy!\n");
    //    iterate_pci();
    //}
    init_apic();
    init_ioapic();
    init_sched();
    enable_interrupts();
    init_ps2();
    // kickstart the "scheduler"
    //__asm__("int $32");
    hlt();
}
