#include "font_renderer.h"
#include "font.h"
#include <stdint.h>

static struct limine_framebuffer* fb;

void InitFB(struct limine_framebuffer* _fb)
{
    fb = _fb;   
}

void renderChar(char chr,int x,int y,int scale,int color)
{
    int width = CHAR_WIDTH * scale;
    int height = CHAR_HEIGHT  * scale;
    // Note: we assume the framebuffer model is RGB with 32-bit pixels.
    for (int _y = y; _y < y + height; _y++) {
        for (int _x = x; _x < x + width; _x++) {
        uint32_t *fb_ptr = fb->address;
        if (((letters[chr - 32][CHAR_HEIGHT - 1 - ((_y % height)/scale)] >> CHAR_WIDTH-(_x % width)/scale) & 1) != 0) 
            fb_ptr[_y * (fb->pitch / 4) + _x] = color;

        }
    }
}

