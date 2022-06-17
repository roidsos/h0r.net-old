#include <io.h>
int kernel_main(){
    setCursorpos(0);
    printc("\nlol ",0x04);
    print("Hello World!\nim a new line!\n");
    print(hex2str(0x11111111));
}
