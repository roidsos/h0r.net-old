#include <io.h>
#include <gdt/gdt.h>
#include <interrupts/IDT.h>

extern "C" int kernel_main(){
    Clearscr(0x4f);
    GDT gdt;
    print("lol\n",0x24);
    print("Hello World!\nim a new line!");
    IDT idt(&gdt);
    idt.Activate();
}