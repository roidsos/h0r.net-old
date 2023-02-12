#include "io/io.h"
#include <drivers/arch/x86/GDT/gdt.h>


#include <drivers/mouse.h>
#include <lib/printf.h>
#include <util/string.h>
#include <util/colors.h>
#include <drivers/driver.h>
#include <drivers/mass-storage.h>
#include <drivers/mouse.h>
#include <drivers/VGA.h>
#include <render/renderer.h>
#include <drivers/PIT.h>
#include <drivers/pc-speaker.h>
#include <desh/desh.h>
extern "C" int kernel_main()
{
    Clearscr(LIGHT_BLUE);
    enable_text_cursor(14, 15);

//gdts are fucked
//    GDT gdt;
//
//    gdt.AddSegment(0, 0, 0);//null
//    gdt.AddSegment(0x00AF, 0x000FFFFF, 0x9B);//code
//    gdt.AddSegment(0x00AF, 0x000FFFFF, 0x93);//data
//    gdt.AddSegment(0, 0x000FFFFF, (GDT_DATA_PL3));
//
//    gdt.Load();

    InitDrivers();
//    uint_8 test[512];
//    ATA::Read28(0,0,test,1);
//    for(int i = 0; i <= 0x01FF; i++)
//    {
//        printf("0x%x ",test[i]);
//    }
//    printf("\n");

    DeshInit();

    while (1)
    {
        DeshUpdate();
    }
}