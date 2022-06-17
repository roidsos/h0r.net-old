#include <io.h>
int kernel_main(){
    setCursorpos(0);
    printc("lol ",0x04);
    print("Hello World!");
}
