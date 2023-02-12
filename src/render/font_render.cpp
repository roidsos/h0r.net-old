#include <drivers/VGA.h>
#include <render/font_render.h>
#include "util/font.h"

void renderFont(uint_16 _x, uint_8 _y, uint_8 color, char letter) {
    VGA vga;
    for (int y = 0; y < 13; y++) {
        for (int x = 0; x < 16; x++) {
            if (((letters[letter][12 -y] >> (15 - x)) & 1) == 1) {
                vga.PutPixel(_x + x,_y + y, color);
            }
        }
    }
}
void renderString(uint_16 _x, uint_8 _y,int width, uint_8 color, char* str){
    for(int i = 0;str[i] != 0;i++){
        if(str[i] == 96){
            renderFont((i%16)*16,i/16*13,0,0);
            continue;
        }
        renderFont( _x +(i%width)*10, _y + i/width*13,color, str[i]);
    }
}
