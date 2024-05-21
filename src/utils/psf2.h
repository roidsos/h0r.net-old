#ifndef __PSF2_H__
#define __PSF2_H__
#include <uterus.h>
#include <limine.h>

//WARN: Arch specific code in utils/ + TODO: smh move to arch/ or make architecture neutral
struct uterus_context *init_uterus_with_psf2_font(struct limine_file* psf2,struct limine_framebuffer* fb);

#endif // __PSF2_H__