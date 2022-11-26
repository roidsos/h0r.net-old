#include "io/io.h"
//#include <gdt/gdt.h>
//#include <memory/memory.h>

#include <drivers/mouse.h>
#include <util/printf.h>
#include <util/string.h>
#include <util/colors.h>
#include <drivers/driver.h>
#include <drivers/mouse.h>
#include <drivers/VGA.h>
#include <render/renderer.h>
#include <drivers/PIT.h>
#include <drivers/pc-speaker.h>

extern "C" int kernel_main()
{
    Clearscr(LIGHT_BLUE);
    enable_text_cursor(14, 15);

    VGA vga;

    //vga.SetMode(320,200,8);
    //vga.Clearscr(0); // forgot xD
    //window(10,3,140,70,"BENIS",4,3);
    PIT::PitInit();
    InitDrivers();
    pcspeaker p;
    while (1)
    { 
        //vga.PutPixel(mousex,mousey,4);
    }
}
// TODO: fix gdts

// GDT gdt;

// gdt.AddSegment(0, 0, 0);
// gdt.AddSegment(0x00AF, 0x000FFFFF, 0x9B);//code
// gdt.AddSegment(0x00AF, 0x000FFFFF, 0x93);//data
// gdt.AddSegment(0, 0x000FFFFF, (GDT_DATA_PL3));
//
// gdt.Load();