#include <io.h>
#include <gdt/gdt.h>
#include <drivers/PIT.h>
#include <render/font_render.h>
#include <interrupts/IDT.h>
#include <memory/Heap.h>
#include <memory/memory.h>
//#include <drivers/ata.h>
#include <drivers/pc-speaker.h>
#include <drivers/VGA.h>
#include <render/renderer.h>
#include <drivers/keyboard.h>
#include "colors.h"
#include "snake.h"

void* keybuffer;

extern "C" int kernel_main(){
     pcspeaker sp;

     InitHeap(0x100000,0x100000);//initialize the heap

     InitIDT();//initialize the IDT


    keybuffer = calloc(2);

    Clearscr(0x0F);


    VGA vga;
    vga.SetMode(320, 200, 8);

    Snake::Init();
    enable_text_cursor(14, 15);

    // static uint_16* pixel = (uint_16*)0xa0000;

    // for (int i = 0; i < 320 * 200; i++) {
    //     pixel[i] = 0x7;
    // }
    // unsigned char* VGAPIXELS = 0xa0000;
    // for (int i = 0; i < 320 * 200; i++) {
    //     VGAPIXELS[i] = 0x7;
    // }
    // *pixel = 0xE;
    // pixel++;
    // *pixel = 0xE;
    // *pixel++ = 0x2;
    // pixel++;
    // ++pixel;
    // ++*pixel = 0x2;
    // *pixel++ = 0x2;
    char str[] {
        55,//W
        69,//e
        76,//l
        67,//c
        79,//o
        77,//m
        69,//e
        96,// 
        69,//e
        0
    };






    while(1){
        Snake::Update();
        // PIT::Sleep(1000);
        // print(hex2str((uint_64)PIT::()));
        // print("       ");
        // setCursorpos(0);
        //sp.beep();
    }
}