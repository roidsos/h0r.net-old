#include <interrupts/IDT.h>
#include <io.h>

extern IDT64 _idt[256];
extern uint_64 isr1;
extern "C" void loadIDT();

void initidt(){
    for(uint_8 t = 0; t < 256;t++){
    _idt[t].zero = 0;
    _idt[t].offset_low  = (uint_16)(((uint_64)&isr1 & 0x000000000000ffff));
    _idt[t].offset_mid  = (uint_16)(((uint_64)&isr1 & 0x00000000ffff0000) >> 16);
    _idt[t].offset_high = (uint_32)(((uint_64)&isr1 & 0xffffffff00000000) >> 32);
    _idt[t].ist = 0;
    _idt[t].selector = 0x08;
    _idt[t].type_attr = 0x8e;

    } 
    outb(0x21,0xfd);
    outb(0xa1,0xff);
    loadIDT();

}
extern "C" void isr1_handler(){
    print(hex2str(inb(0x60)));
    outb(0x20,0x20);
    outb(0xa0,0x20);
}