#include <io.h>
#include <gdt/gdt.h>
#include <drivers/PIT.h>
#include <render/frender.h>
#include <interrupts/IDT.h>
#include <memory/Heap.h>
#include <memory/memory.h>
//#include <drivers/ata.h>
#include <drivers/pc-speaker.h>
#include <drivers/VGA.h>
#include <render/renderer.h>
#include <drivers/keyboard.h>
#include "colors.h"

char* keybuffer;

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
    // printchar('a');
    //enable_text_cursor(14, 15);
    // print("Hello, World!");
    //print("Username: ", 0xF);
    // char* text = input();
    // print("Hello, ");
    // print(text, 0xF);
    // print("\n");
    // print("┌─────────────────┐", 0xF);

        // sp.play_sound(1000);
    //  sp.beep();

//     inb8(0x3DA); // to index state
//     outb8(0x3C0, 0x10); // register index
//     uint_8 value = inb8(0x3C1);
//     print(hex2str((uint_64)value));
//     return;
//     inb8(0x3DA);
//     outb8(0x3C0, 0x10);
//     outb8(0x3C0, value & 0b11110111 || 0b000000100); // disable 4th bit (numbering from 1) to go to 16 background colors


//     Clearscr(0x1f);
//     setCursorpos(27);
//     print("The blue screen of death!",0x1f);
//     setCursorpos(320);
//     print("something went wrong with your device",0x1f);
//     setCursorpos(80 * 24);
//     print("                                                                                ",0xf1);
    // Clearscr(0x0F);
    //print("Hello, World!\n", 0xAF);
    // PIT::SetFrequency(1000);


keybuffer[1] = 0;
    while(1){
        char key =  getch();
        if(key){
            printchar(key);
        }
        // PIT::Sleep(1000);
        //print(hex2str((uint_64)PIT::TimeSinceBoot));
        setCursorpos(0);
        //sp.beep();
    }
}