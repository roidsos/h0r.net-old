#include <io.h>
extern "C" void musint(){
    uint_8 status = inb8(0x64);
    if(! status&0x20)
        return;

    inb8(0x60);
    print("E");
    outb8(0x20,0x20);
    outb8(0xa0,0x20);
}