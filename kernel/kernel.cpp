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

void* keybuffer;

extern "C" int kernel_main(){
     pcspeaker sp;
    //GDT gdt;
     InitHeap(0x100000,0x100000);//initialize the heap

    InitIDT();//initialize the IDT


    keybuffer = calloc(2);

    //VGA vga;
//
    //char str[] {
    //    44,//L
    //    79,//o
    //    83,//s
    //    69,//e
    //    82,//r
    //    0
    //};
    // vga.SetMode(320, 200, 8);
    // vga.Clearscr(0x0);
    // window(0,0,150,70,"EXPLORER",0x0,RED);
    // window(75,60,150,70,"GAME",0x0,RED);
    // renderString(0, 0, (uint_8)320, 0xF, str);
    //RenderCircle(30,30,10,BLUE);
    //PIT::SetFrequency(1);
    Clearscr(0x0F);

    InitIDT();

    VGA vga;


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
    //vga.SetMode(320, 200, 8);
    // vga.Clearscr(0x1);
    // window(0,0,150,70,"EXPLORER",0x0,RED);
    // window(75,60,150,70,"GAME",0x0,RED);
    // renderString(0, 0, 320, 0xF, str);
    //RenderCircle(30,30,10,BLUE);
    //PIT::SetFrequency(1);
    // Clearscr(0x0F);
    // printchar('a');
    // enable_text_cursor(14, 15);
    // print("Hello, World!");
    //print("Username: ", 0xF);
    // char* text = input();
    // print("Hello, ");
    // print(text, 0xF);
    // print("\n");
    // print("┌─────────────────┐", 0xF);





    while(1){
        // PIT::Sleep(1000);
        // print(hex2str((uint_64)PIT::()));
        // print("       ");
        // setCursorpos(0);
        //sp.beep();
    }
}