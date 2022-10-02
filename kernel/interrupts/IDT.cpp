#include <interrupts/IDT.h>

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

extern "C" void isrC();
extern "C" void isr1();
extern "C" void isr0();
extern "C" void isrIgnore();
extern "C" void loadidt();
void RemapPIC(){
    uint_8 a1,a2;
    a1 = inb8(PIC1_DATA);
    IOWait();
    a2 = inb8(PIC2_DATA);
    IOWait();
    
    outb8(PIC1_COMMAND,ICW1_INIT | ICW1_ICW4);
    IOWait();    
    outb8(PIC2_COMMAND,ICW1_INIT | ICW1_ICW4);
    IOWait();

    outb8(PIC1_DATA,0);
    IOWait();
    outb8(PIC2_DATA,8);
    IOWait();

    outb8(PIC1_DATA,4);
    IOWait();
    outb8(PIC2_DATA,2);
    IOWait();

    outb8(PIC1_DATA,ICW4_8086);
    IOWait();
    outb8(PIC2_DATA,ICW4_8086);
    IOWait();

    outb8(PIC1_DATA,a1);
    IOWait();
    outb8(PIC2_DATA,a2);
}
//initializes and loads the IDT
void InitIDT()
{
    AddEntry((void*)&isr0,0b10001110,0);
    AddEntry((void*)&isr1,0b10001110,1);
    AddEntry((void*)&isrC,0b10001110,12);

    RemapPIC();
    loadidt();

    //TODO: unstupidify the fuking code
    if(inb8(0x64) & 0x1)// initialize the mouse
        inb8(0x60);
    outb8(0x64,0x60);
    outb8(0x60,0b01100111);
    outb8(0x64,0xA8);

    outb8(0x64,0xd4);
    outb8(0x60,0xf4);
    inb8(0x60);

    
    


    
}
//activates the IDT
void ActivateIDT()
{
     asm("sti");

    outb8(0x21,0xfd);
    outb8(0xa1,0xff);
}
//Adds an entry to the IDT
void AddEntry(void* Isr,int attr,int _num){
 _idt[_num].zero = 0;
 _idt[_num].ist = 0;
 _idt[_num].offset_low  = (uint_16)(((uint_64)Isr & 0x000000000000ffff));
 _idt[_num].offset_mid  = (uint_16)(((uint_64)Isr & 0x00000000ffff0000) >> 16);
 _idt[_num].offset_high = (uint_32)(((uint_64)Isr & 0xffffffff00000000) >> 32);
 _idt[_num].selector = 8;
 _idt[_num].types_attr = attr;
}
