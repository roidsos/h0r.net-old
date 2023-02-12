#include "drivers/arch/x86/GDT/gdt.h"
#include "io/io.h"
#include <drivers/memory/memory.h>
extern "C" uint_64  _gdt[256];
extern "C" void loadgdt();
void create_descriptor(uint_32 num, uint_32 base, uint_32 limit, uint_16 flag)
{
    // code copied from https://wiki.osdev.org/GDT_Tutorial
    // Create the high 32 bit segment
    _gdt[num]  =  limit       & 0x000F0000;         // set limit bits 19:16
    _gdt[num] |= (flag <<  8) & 0x00F0FF00;         // set type, p, dpl, s, g, d/b, l and avl fields
    _gdt[num] |= (base >> 16) & 0x000000FF;         // set base bits 23:16
    _gdt[num] |=  base        & 0xFF000000;         // set base bits 31:24
 
    // Shift by 32 to allow for low part of segment
    _gdt[num] <<= 32;
 
    // Create the low 32 bit segment
    _gdt[num] |= base  << 16;                       // set base bits 15:0
    _gdt[num] |= limit  & 0x0000FFFF;               // set limit bits 15:0
}
GDT::GDT()
{   

    memset(_gdt,0,0xfff);
    fullness = 0; 
}

void GDT::AddSegment(uint_32 base, uint_32 limit, uint_16 flag)
{
    create_descriptor(fullness++,base,limit,flag);
}

void GDT::Load()
{

    loadgdt();

}


