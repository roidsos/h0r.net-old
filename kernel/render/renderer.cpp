#include <render/renderer.h>
#include <render/font_render.h>
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

uint_8 strlen(char* str){
    uint_8 i = 0;
    while(str[i] != 0){
    i++;
    }
    return i;
}

uint_8 strlen(const char* str){
    uint_8 i = 0;
    while(str[i] != 0){
    i++;
    }
    return i;
}

uint_8 sqrt(uint_8 x){
    if(x == 0 || x == 1){
        return x;
    }

    uint_8 start = 1, end = (uint_8)256;

    while (start + 1 < end){
        uint_8 mid = start + (end - start) / 2;
        if(mid == x/mid){
            return mid;
        } else if(mid > x/mid){
            end = mid;
        } else{
            start = mid;
        }
    }      
    return start;
}

uint_8 abs(int_8 input){
    return (uint_8) input < 0 ? input * -1 : input;   
}
 

void window(uint_16 x, uint_8 y, uint_16 width, uint_8 height, const char* title, uint_8 border_color, uint_8 inside_color) {
    uint_8 tlen = strlen(title);
    uint_8 rows = tlen / (width / 16) + (tlen % 16 != 0);
    filledrect(x, y, width, rows*16, inside_color);
    filledrect(x, y+rows*16, width, height, inside_color);
    renderString(x,y + 2,width/16,0x1,title);
    rect(x, y, width, 16, border_color);
    rect(x, y+rows*16, width, height, border_color);
    renderString(x + 2,y + 2,width/16,0x1,title);
}

void RenderCircle(uint_16 centerx,uint_16 centery,uint_8 radius,uint_8 color){
VGA vga;
    for(int x = centerx - radius;x < centerx + radius;x++){
        for(int y = centery - radius;y < centery + radius;y++){
            uint_8 diffx = abs(x - centerx);
            uint_8 diffy = abs(y - centery);

            uint_8 distance = sqrt(diffx*diffx+diffy*diffy);
            if(distance <= radius)
                vga.PutPixel(x,y, color);

        }
    }
}