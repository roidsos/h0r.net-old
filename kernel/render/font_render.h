#ifndef __font_render_H__
#define __font_render_H__

void renderFont(uint_16 _x, uint_8 _y, uint_8 color, char letter);
void renderString(uint_16 _x, uint_8 _y,int width, uint_8 color, char* str);
void renderString(uint_16 _x, uint_8 _y,int width, uint_8 color, const char* str);

#endif