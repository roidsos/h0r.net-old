#include <drivers/VGA.h>
#include <render/frender.h>
#include "font.h"

void renderFont(uint_16 _x, uint_8 _y, char letter) {
    VGA vga;
    vga.PutPixel(0, 0, 0xF);
    for (int y = 0; y < 13; y++) {
        for (int x = 0; x < 16; x++) {
            if (((letters[letter][12 -y] >> (15 - x)) & 1) == 1) {
                vga.PutPixel(_x + x,_y + y, 0xF);
            } else {
                vga.PutPixel(_x + x,_y + y, 0x0);
            }
        }
    }
}
void renderString(uint_16 _x, uint_8 _y,uint_8 width, char* str){
    for(int i = 0;str[i] != 0;i++){
        if(str[i] == 96){
            renderFont((i%16)*16,i/16*13,0);
            continue;
        }
        renderFont((i%width)*16,i/width*13, str[i]);
    }
}