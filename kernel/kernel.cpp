#include <io.h>
#include <gdt/gdt.h>
#include <interrupts/IDT.h>
#include <memory/Heap.h>
#include <memory/memory.h>
#include <drivers/ata.h>
extern "C" int kernel_main(){
    Clearscr(0x4f);
    GDT gdt;
    print("lol\n",0x24);
    print("Hello World!\nim a new line!\n");
    InitHeap(0x100000,0x100000);



    IDT idt(&gdt);
    idt.Activate();
}