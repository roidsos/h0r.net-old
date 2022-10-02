#include "io/io.h"
#include <interrupts/IDT.h>
//#include <gdt/gdt.h>
#include <memory/Heap.h>
//#include <memory/memory.h>
#include <drivers/soundblaster.h>
#include <util/printf.h>
#include <util/string.h>
#include <drivers/ata.h>
#include <drivers/pci.h>
#include <drivers/PIT.h>
#include <filesystem/msdospart.h>
#include <util/colors.h>
#include <util/logger.h>
#include <drivers/pc-speaker.h>
void *keybuffer;


extern "C" int kernel_main()
{

    Clearscr(0x0F);

    InitHeap(0x100000, 0x100000); // initialize the heap
    LogINFO("Initalized heap \n");
    
    init_SB16();
    LogINFO("Initalized SoundBlaster 16 (Not-Even-Half-Done) \n");
    InitIDT();
    LogINFO("Initalized IDT \n");
    keybuffer = malloc(1);
    LogINFO("Initalized keyboard \n");
    ActivateIDT();
    LogINFO("Activated IDT \n");
    PIT::PitInit();
    LogINFO("Initialized PIT \n");
    enable_text_cursor(14, 15);
    PCI pci;
    pci.SelectDrivers();
    ATA ata2(0x1F0, true);
    LogINFO("Initalized ATA \n");
    //LoadMBR(ata2);
    //LogINFO("Loaded Fat\n");
    //print("hello world!\nthis is a custom-built kernel called h0r.net");
    //pcspeaker sp;
    //sp.play_sound(1000);
    while (1)
    {
        PIT::Sleep(1000);
        print("a");
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