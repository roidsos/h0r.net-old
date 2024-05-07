#include <backends/fb.h>
#include <limine.h>
#include <stdint.h>
#include <uterus.h>

#define PSF2_MAGIC0 0x72
#define PSF2_MAGIC1 0xb5
#define PSF2_MAGIC2 0x4a
#define PSF2_MAGIC3 0x86

typedef struct {
    uint8_t magic[4];
    uint32_t version;
    uint32_t headerSize;
    uint32_t flags;
    uint32_t length;
    uint32_t charSize;
    uint32_t height, width;
} __attribute__((packed)) psf2Hdr;

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
                          NULL, NULL, NULL, NULL, NULL, NULL,psf2buf, hdr.width, hdr.height);
}