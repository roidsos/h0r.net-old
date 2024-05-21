#include <backends/fb.h>
#include <limine.h>
#include <stdint.h>
#include <uterus.h>

#define PSF2_MAGIC0 0x72
#define PSF2_MAGIC1 0xb5
#define PSF2_MAGIC2 0x4a
#define PSF2_MAGIC3 0x86

typedef struct {
    u8 magic[4];
    u32 version;
    u32 headerSize;
    u32 flags;
    u32 length;
    u32 charSize;
    u32 height, width;
} __attribute__((packed)) psf2Hdr;

//WARN: Arch specific code in utils/ + TODO: smh move to arch/ or make architecture neutral
struct uterus_context *
init_uterus_with_psf2_font(struct limine_file *psf2,
                           struct limine_framebuffer *fb) {
    char *psf2buf = psf2->address;
    psf2Hdr hdr = *(psf2Hdr *)psf2->address;
    psf2buf += hdr.headerSize;

    if (hdr.magic[0] != 0x72 || hdr.magic[1] != 0xb5 || hdr.magic[2] != 0x4a ||
        hdr.magic[3] != 0x86)
        return (void *)0;

    return uterus_fb_init(fb->address, fb->width, fb->height, fb->pitch, NULL,
                          NULL, NULL, NULL, NULL, NULL, NULL, psf2buf,
                          hdr.width, hdr.height);
}