#include <drivers/arch/x86/IDT/IDT.h>
#include <drivers/arch/x86/x86.h>
#include <drivers/memory/Heap.h>
#include <drivers/mass-storage.h>
#include <drivers/pciide.h>
#include <drivers/pci.h>
#include <drivers/PIT.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/soundblaster.h>
#include <drivers/filesystem/msdospart.h>
#include <util/logger.h>
#include <lib/printf.h>

void InitDrivers(){


    InitIDT();
    LogINFO("Initalized IDT \n");

    InitHeap(0x100000, 0x100000); 
    LogINFO("Initalized heap \n");

    PIT::PitInit();
    LogINFO("Initalized PIT \n");

    initkeyboard();
    initmouse();
    LogINFO("Initalized keyboard & mouse \n");
    
    ActivateIDT();
    
    //init_SB16();
    //printf("soundblaster version: %i.%i\n" ,sb16_version_major,sb16_version_minor);
    //LogINFO("Initalized SoundBlaster 16 (Not-Even-Half-Done) \n");

    PCI::SelectDrivers();
    LogINFO("Initalized PCI \n");

    x86_Read_From_Drive();

    ATA::init();

    LogINFO("Initalized Mass Storage \n");
    LoadMBR();
    LogINFO("Loaded Fat\n");
}