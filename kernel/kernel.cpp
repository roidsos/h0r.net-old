#include <io.h>
#include <gdt/gdt.h>
#include <interrupts/IDT.h>
#include <memory/Heap.h>
#include <memory/memory.h>
extern "C" int kernel_main(){
    Clearscr(0x4f);
    GDT gdt;
    print("lol\n",0x24);
    print("Hello World!\nim a new line!\n");
    InitHeap(0x100000,0x100000);
uint_64* test = (uint_64*)aligned_alloc(0x4000,0x08);
print(hex2str(test));print("\n");
free(test);
uint_64* test2 = (uint_64*)aligned_alloc(0x4000,0x08);
print(hex2str(test2));print("\n");
uint_64* test3 = (uint_64*)realloc(test2,0x08);
print(hex2str(test3));
    //IDT idt(&gdt);
    //idt.Activate();
}