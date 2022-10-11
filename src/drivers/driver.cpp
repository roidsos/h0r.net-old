#include <drivers/arch/x86/IDT/IDT.h>
#include <drivers/memory/Heap.h>
#include <drivers/ata.h>
#include <drivers/pci.h>
#include <drivers/PIT.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/soundblaster.h>
#include <drivers/filesystem/msdospart.h>
#include <util/logger.h>
#include <util/printf.h>

void InitDrivers(){
    InitIDT();
    LogINFO("Initalized IDT \n");

    InitHeap(0x100000, 0x100000); 
    LogINFO("Initalized heap \n");

    PIT::PitInit();
    LogINFO("Initialized PIT \n");

    initkeyboard();
    initmouse();
    LogINFO("Initalized keyboard & mouse \n");
    
    ActivateIDT();
    LogINFO("Activated IDT \n");
    
    init_SB16();
    printf("soundblaster version: %i.%i\n" ,sb16_version_major,sb16_version_minor);
    LogINFO("Initalized SoundBlaster 16 (Not-Even-Half-Done) \n");

    PCI::SelectDrivers();
    LogINFO("Initalized PCI \n");

    ATA::Init();
    ATA::ListDevices();
    LogINFO("Initalized ATA \n");

    //LoadMBR(ata2);
    //LogINFO("Loaded Fat\n");
}