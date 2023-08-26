#include "../limine/limine.h"

#include "vendor/multiboot.h"
#include "utils/screen.h"

#include <stdint.h>
#include <stddef.h>

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

void _start(void) {
    //// Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }
    //// Ensure we got a memmap.
    if (framebuffer_request.response == NULL) {
        hcf();
    }

    // Fetch the first framebuffer.
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    // Fetch the first framebuffer.
    struct limine_memmap_response *memmap_resp = framebuffer_request.response;
    
    InitScreen(framebuffer);


    printf_("Memmap entry count: %i\n",memmap_resp->entry_count);
    for(int i=0; i < memmap_resp->entry_count;i++){
        printf_("  Memmap entry #%i: Base: 0x%x Length: 0x%x \n",i
			,memmap_resp->entries[i]->base
			    ,memmap_resp->entries[i]->length);

    }
    
    hcf();
}
