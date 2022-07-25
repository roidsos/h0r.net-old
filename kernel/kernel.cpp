#include <io.h>
#include <gdt/gdt.h>
#include <drivers/PIT.h>
#include <render/frender.h>
#include <interrupts/IDT.h>
#include <memory/Heap.h>
#include <memory/memory.h>
#include <drivers/ata.h>
#include <drivers/pc-speaker.h>
#include <drivers/VGA.h>
#include <render/renderer.h>
#include "colors.h"

extern "C" int kernel_main(){
     pcspeaker sp;
    //GDT gdt;
//     InitHeap(0x100000,0x100000);
    // sp.play_sound(1000);
     // sp.beep();

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
    //PIT::SetFrequency(1000);
    //InitIDT();
    ATA ata(0x1F0,true);
    ata.Identify();
    //VGA vga;
    //char str[] {
    //    44,//L
    //    79,//o
    //    83,//s
    //    69,//e
    //    82,//r
    //    0
    //};
    //vga.SetMode(320, 200, 8);
    //vga.Clearscr(0x1);
    ////renderString(0, 0, (uint_8)320, 0xF, str);
    //window(0,0,150,70,"EXPLORER",0x0,RED);
    //window(75,60,150,70,"GAME",0x0,RED);
    ////RenderCircle(30,30,10,BLUE);
    ////PIT::SetFrequency(1);
    


    while(1){
        // print("A");
        // PIT::Sleep(1000);
        //print(hex2str((uint_64)PIT::TimeSinceBoot));
        //setCursorpos(0);
        //sp.beep();
    }
}