#include "font_renderer.h"
#include "font.h"
#include "utils/types.h"

static struct limine_framebuffer* fb;

void InitFB(struct limine_framebuffer* _fb)
{
    fb = _fb;   
}

void renderChar(char chr,int x,int y,int scale,int color)
{
    int width = 8 * scale;
    int height =  13 * scale;
    // Note: we assume the framebuffer model is RGB with 32-bit pixels.
    for (int _y = y; _y < y + height; _y++) {
        for (int _x = x; _x < x + width; _x++) {
        uint_32 *fb_ptr = fb->address;
        if (((letters[chr - 32][13 - ((_y % height)/scale)] >> 7-(_x % width)/scale) & 1) != 0) 
            fb_ptr[_y * (fb->pitch / 4) + _x] = color;

        }
    }
}

