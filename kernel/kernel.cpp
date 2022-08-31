
#include <io/io.h>
#include <interrupts/IDT.h>
#include <memory/Heap.h>
#include <memory/memory.h>
#include <drivers/soundblaster.h>
#include <util/printf.h>

void *keybuffer;

extern "C" int kernel_main(){

    InitHeap(0x100000,0x100000);//initialize the heap

    init_SB16();

    InitIDT();//initialize the IDT


    

   keybuffer = malloc(1);


    ActivateIDT();

    Clearscr(0x0F);

    enable_text_cursor(14, 15);
    printf("soundblaster version: %i.%i",sb16_version_major,sb16_version_minor);

   while(1){}

}