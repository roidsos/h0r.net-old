#ifndef __FONT_RENDERER_H__
#define __FONT_RENDERER_H__

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 13

#include "../limine/limine.h"

void fr_init(struct limine_framebuffer* fb);
void fr_render_char(char chr,int x,int y,int scale,int color);
void fr_clear_screen(int color);
void fr_render_string(char* str, int x, int y,int scale, int color);
void fr_render_centered_string(char* string, int y, int scale, int color);

#endif // __FONT_RENDERER_H__
