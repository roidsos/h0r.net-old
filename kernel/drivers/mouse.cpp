#include <io/io.h>
void initmouse(){
    
    outb8(0x64,0xA8);

    outb8(0x64,0xd4);
    outb8(0x60,0xf4);
    inb8(0x60);
}

//this does not even work
extern "C" void musint(){
    uint_8 status = inb8(0x64);
    if(! status&0x20)
        return;

    inb8(0x60);
    print("E");
    outb8(0x20,0x20);
    outb8(0xa0,0x20);
}