#ifndef __FONT_RENDERER_H__
#define __FONT_RENDERER_H__

#include "../limine/limine.h"

void InitFB(struct limine_framebuffer* fb);
void renderChar(char chr,int x,int y,int scale,int color);

#endif // __FONT_RENDERER_H__