#ifndef __FONT_RENDERER_H__
#define __FONT_RENDERER_H__

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 13

#include "../limine/limine.h"

void InitFB(struct limine_framebuffer* fb);
void renderChar(char chr,int x,int y,int scale,int color);
void ClearScreen(int color);
void basicStringRender(char* str, int x, int y,int scale, int color);
void centeredStringRenderer(char* string, int y, int scale, int color);

#endif // __FONT_RENDERER_H__
