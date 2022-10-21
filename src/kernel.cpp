#include "io/io.h"
//#include <gdt/gdt.h>
//#include <memory/memory.h>

#include <drivers/mouse.h>
#include <util/printf.h>
#include <util/string.h>
#include <util/colors.h>
#include <drivers/driver.h>
#include <drivers/PIT.h>
#include <drivers/pc-speaker.h>

extern "C" int kernel_main()
{
    Clearscr(LIGHT_BLUE);
    enable_text_cursor(14, 15);


    InitDrivers();


    pcspeaker sp;
    sp.beep();

    //print("hello world!\nthis is a custom-built kernel called h0r.net");
    // play_sound()
    while (1)
    {
        if(mousebl || mousebr){
            printf("click");
            mousebl = false;
            mousebr = false;
        }
        PIT::Sleep(5);
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