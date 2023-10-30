#ifndef __PSF2_H__
#define __PSF2_H__
#include <flanterm.h>
#include <limine.h>

struct flanterm_context *init_flanterm_with_psf2_font(struct limine_file* psf2,struct limine_framebuffer* fb);

#endif // __PSF2_H__