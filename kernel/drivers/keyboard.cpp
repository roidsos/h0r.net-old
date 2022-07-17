#include <drivers/keyboard.h>
extern "C" keyint(){
    //print(int2str((uint_64)inb8(0x60)));
    outb8(0x20,0x20);
    outb8(0xa0,0x20);
};
