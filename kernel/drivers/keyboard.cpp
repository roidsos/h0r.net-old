#include <drivers/keyboard.h>

uint_8 inportb(uint_16 port) {
    uint_8 r;
    asm("inb %1, %0" : "=a" (r) : "dN" (port));
    return r;
}

extern "C" void keyint(){
    uint_16 scancode = (uint_16)inportb(0x60);
    outb8(0x20,0x20);
    outb8(0xa0,0x20);

};
