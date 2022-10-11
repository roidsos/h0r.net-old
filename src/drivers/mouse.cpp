#include <io/io.h>
#include <util/printf.h>
uint_8 mousey,mousex;
bool mousebl,mousebr;
uint_8 offset;
void set_sample_rate(uint_8 sr){
    outb8(0xD4, 0x64);                      // tell the controller to address the mouse
    outb8(0xF3, 0x60);                      // write the mouse command code to the controller's data port
    while(!inb8(0x64) & 1){}                // wait until we can read
    inb8(0x60);                             // read back acknowledge. This should be 0xFA
    outb8(0xD4, 0x64);                      // tell the controller to address the mouse
    outb8(sr, 0x60);                       // write the parameter to the controller's data port
    while(!inb8(0x64) & 1){}                // wait until we can read
    inb8(0x60);                             // read back acknowledge. This should be 0xFA
}

void initmouse(){

    mousex = 0;
    mousey = 0;

    mousebl = false;
    mousebr = false;
    
    outb8(0x64,0xA8);

    outb8(0x64,0xd4);
    outb8(0x60,0xf4);
    inb8(0x60);

    //set_sample_rate(100);
}

//this does not even work
extern "C" void musint(){
    uint_8 status = inb8(0x64);
    if(! status&0x20)
        return;
    uint_8 packet[3];

    packet[offset] = inb8(0x60);
    
    offset = (offset + 1) % 3;

    if(offset == 0){
    mousebl = packet[0] & 0x00000001;
    mousebr = packet[0] & 0x00000010;

    mousex += packet[1]- ((packet[0] << 4) & 0x100);
    mousey += packet[2]- ((packet[0] << 3) & 0x100);

    printf("mouse at x: %i y: %i", mousex,mousey);
    }
    outb8(0x20,0x20);
    outb8(0xa0,0x20);
}