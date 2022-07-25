#include <drivers/keyboard.h>
extern "C" void keyint(){
    print(int2str((uint_8)inb8(0x60)));
};
