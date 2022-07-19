#ifndef __RENDERER_H__
#define __RENDERER_H__
#include <stdint.h>

void filledrect(uint_16 x, uint_8 y, uint_16 width, uint_8 height, uint_8 color);
void rect(uint_16 x, uint_8 y, uint_16 width, uint_8 height, uint_8 color);
void window(uint_16 x, uint_8 y, uint_16 width, uint_8 height, char* title, uint_8 border_color);

#endif