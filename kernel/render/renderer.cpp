#include <render/renderer.h>
#include <drivers/VGA.h>

void filledrect(uint_16 x, uint_8 y, uint_16 width, uint_8 height, uint_8 color) {
    VGA vga;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            vga.PutPixel(x + j, y + i, color);
        }
    }
}

void rect(uint_16 x, uint_8 y, uint_16 width, uint_8 height, uint_8 color) {
    VGA vga;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                vga.PutPixel(x + j, y + i, color);
            }
        }
    }
}

void window(uint_16 x, uint_8 y, uint_16 width, uint_8 height, char* title, uint_8 border_color) {
    filledrect(x, y, width, 10, 0xF);
    rect(x, y, width, 10, border_color);
    filledrect(x, y+10, width, height-11, 0xF);
    rect(x, y+9, width, height-10, border_color);
}