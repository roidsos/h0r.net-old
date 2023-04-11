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

    GDT gdt;
    gdt.AddSegment(0, 0, 0);//null
    gdt.AddSegment(0x00, 0x000FFFFF, GDT_CODE_PL0);//64 bit pl 0 code segment
    gdt.AddSegment(0x00, 0x000FFFFF, GDT_DATA_PL0);//64 bit pl 0 data segment
    gdt.AddSegment(0x00, 0x000FFFFF, GDT_CODE_PL3);//64 bit pl 3 code segment
    gdt.AddSegment(0x00, 0x000FFFFF, GDT_DATA_PL3);//64 bit pl 3 data segmen
    gdt.AddSegment(0x00, 0x000FFFFF, GDT16_CODE_PL0);//16 bit pl 3 code segment
    gdt.AddSegment(0x00F, 0x000FFFFF, GDT16_DATA_PL0);//16 bit pl 3 data segmen

    gdt.Load();

    InitDrivers();
//    uint_8 test[512];
//    ATA::Read28(0,0,test,1);
//    for(int i = 0; i <= 0x01FF; i++)
//    {
//        printf("0x%x ",test[i]);
//    }
//    printf("\n");

    //Clearscr(LIGHT_BLUE);
    //enable_text_cursor(14, 15);
    
    DeshInit();
    asm("int $0x10");
    while (1)
    {
        DeshUpdate();
    }
}
