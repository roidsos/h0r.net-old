#include <drivers/VGA.h>
#include <render/frender.h>
#include "font.h"

void renderFont(uint_16 x, uint_8 y, char letter) {
    VGA vga;
    vga.PutPixel(0, 0, 0xF);
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 8; x++) {
            if (((letters[letter][y] >> x) & 1) == 1) {
                vga.PutPixel(x, y, 0xF);
            } else {
                vga.PutPixel(x, y, 0x0);
            }
        }
    }
}