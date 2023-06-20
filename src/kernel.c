#include "../limine/limine.h"

#include "vendor/multiboot.h"
#include "utils/screen.h"

#include <stdint.h>
#include <stddef.h>

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};


void _start(void) {
    //// Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    // Fetch the first framebuffer.
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    InitScreen(framebuffer);


    printf_("Hello Limine!");
    hcf();
}