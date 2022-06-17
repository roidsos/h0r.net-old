#include <io.h>
#include <interrupts/IDT.h>
extern "C" int kernel_main(){
    Clearscr(0x4f);
    print("lol ",0x24);
    print("Hello World!\nim a new line!\n");
}