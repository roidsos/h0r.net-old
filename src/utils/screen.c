#include "screen.h"
#include "vbe/font_renderer.h"

static struct limine_framebuffer* fb;

static int Cursorpos;

static int width;
static int height;

void InitScreen(struct limine_framebuffer* _fb)
{
    Cursorpos = 0;
    fb = _fb;
    width = fb->width/CHAR_WIDTH;
    height = fb->height/CHAR_HEIGHT;

    InitFB(_fb);
}

void RenderChar(char chr,int color)
{
    switch (chr)
    {
	case '\n':
	    Cursorpos += width - (Cursorpos % width) - 1;
	    break;
        
	default:
        int x = (Cursorpos % width) * CHAR_WIDTH;
        int y = (Cursorpos / width) * CHAR_HEIGHT;
        renderChar(chr,x,y,1,color);
        Cursorpos++;

    }
}

void SetCursorpos(int pos)
{
    Cursorpos = pos;
}

void Backspace()
{
    Cursorpos--;
    int x = (Cursorpos % width) * 8;
    int y = (Cursorpos / width) * 13;
    for (int _y = 0; _y < 13; _y++) {
        for (int _x = 0; _x < 8; _x++) {
            int *fb_ptr = fb->address;
                fb_ptr[(y+_y) * (fb->pitch / 4) + (x+_x)] = 0;
        }
    }
}
