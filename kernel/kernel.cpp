
#include <io/io.h>
#include <interrupts/IDT.h>
#include <memory/Heap.h>
#include <memory/memory.h>
#include <drivers/soundblaster.h>

void *keybuffer;

extern "C" int kernel_main(){

    InitHeap(0x100000,0x100000);//initialize the heap

    //init_SB16();

    InitIDT();//initialize the IDT




   keybuffer = malloc(1);


    ActivateIDT();

    Clearscr(0x0F);

    enable_text_cursor(14, 15);
    //print(hex2str((uint_8)sb16_version_major)); 
    //print("\n");
    //print(hex2str((uint_8)sb16_version_minor)); 
    //print("Type here but please dont delete all cuz it bugs out like hell!!!\n UPDATE: not anymore ,there is a bugfix");

   while(1){}

}