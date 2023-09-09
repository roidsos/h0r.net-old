#include "font_renderer.h"
#include "font.h"
#include <arch/x86_64/essential.h>
#include <stdint.h>
#include <utils/string.h>

static struct limine_framebuffer *fb;

void InitFB(struct limine_framebuffer *_fb) { fb = _fb; }
void centeredStringRenderer(char *string, int y, int scale, int color) {
    int offset = (strlen(string) / 2) * CHAR_WIDTH * scale;
    basicStringRender(string, (fb->width / 2) - offset, y, scale, color);
}

void basicStringRender(char *str, int x, int y, int scale, int color) {
    int _x = x;
    int _y = y;
    while (*str) {
        switch (*str) {
        case '\n':
            _y += CHAR_HEIGHT * scale;
            break;

        default:
            renderChar(*str, _x, _y, scale, color);
            _x += CHAR_WIDTH * scale;
            break;
        }
        str++;
    }
}

void renderChar(char chr, int x, int y, int scale, int color) {
    int width = CHAR_WIDTH * scale;
    int height = CHAR_HEIGHT * scale;
    // Note: we assume the framebuffer model is RGB with 32-bit pixels.
    for (int _y = 0; _y < height; _y++) {
        for (int _x = 0; _x < width; _x++) {
            uint32_t *fb_ptr = fb->address;
            if (((letters[chr - 32]
                         [CHAR_HEIGHT - 1 - ((_y % height) / scale)] >>
                      (CHAR_WIDTH - (_x % width) / scale) &
                  1)) != 0)
                fb_ptr[(y + _y) * (fb->pitch / 4) + (x + _x)] = color;
        }
    }
}
void ClearScreen(int color) {
    for (uint64_t i = 0; i < fb->height; i++) {
        for (uint64_t j = 0; j < fb->width; j++) {
            ((int *)fb->address)[i * (fb->pitch / 4) + j] = color;
        }
    }
}
