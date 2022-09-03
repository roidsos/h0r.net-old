
#include <io/io.h>
#include <interrupts/IDT.h>
//#include <gdt/gdt.h>
#include <memory/Heap.h>
//#include <memory/memory.h>
#include <drivers/soundblaster.h>
#include <util/printf.h>
#include <util/string.h>
#include <drivers/ata.h>

void *keybuffer;

extern "C" int kernel_main(){

    InitHeap(0x100000,0x100000);//initialize the heap

    init_SB16();

    //TODO: fix gdts

    //GDT gdt;

    //gdt.AddSegment(0, 0, 0);
    //gdt.AddSegment(0x00AF, 0x000FFFFF, 0x9B);//code
    //gdt.AddSegment(0x00AF, 0x000FFFFF, 0x93);//data
    //gdt.AddSegment(0, 0x000FFFFF, (GDT_DATA_PL3));
//
    //gdt.Load();

    
    InitIDT();//initialize the IDT



   keybuffer = malloc(1);

    ActivateIDT();

    Clearscr(0x0F);

    enable_text_cursor(14, 15);
    printf("master:");
    ATA ata(0x1F0,false);
    ata.Flush();
    ata.Identify();
    printf("slave:");
    ATA ata2(0x1F0,true);
    ata2.Flush();
    ata2.Identify();
    print("\n");

    char* atabuffer = "I am god!!";
    int length = strlen(atabuffer);
    ata2.Write28(0,(uint_8*)atabuffer,length);
    ata2.Flush();

    atabuffer[0] = 0;

    ata2.Read28(0,atabuffer,length);
    ata2.Flush();
    print(atabuffer);
    //printf("soundblaster version: %i.%i",sb16_version_major,sb16_version_minor);

   while(1){}

}