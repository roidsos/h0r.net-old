#include "vendor/multiboot.h"
#include "vendor/printf.h"
#include "drivers/io/vga_buffer.h"
extern "C" int kernel_main(multiboot_info_t* mbi,unsigned int magic){
    clear(0x0F);
    printf("Hello world 0x%x 0x%x",(int)mbi,magic);
    while(true){};
}
