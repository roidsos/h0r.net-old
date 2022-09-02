#include "gdt/gdt.h"
#include "io/io.h"
extern "C" uint_64  _gdt[256];

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
    fullness = 0; 
}

void GDT::AddSegment(uint_32 base, uint_32 limit, uint_16 flag)
{
    create_descriptor(fullness++,base,limit,flag);
}

void GDT::Load()
{
    descriptor[0]   =   fullness * 8;
    descriptor[1]   =   (uint_16)_gdt;
    descriptor[2]   =   (uint_16)((uint_64)_gdt >> 16);
    descriptor[3]   =   (uint_16)((uint_64)_gdt >> 32);
    descriptor[4]   =   (uint_16)((uint_64)_gdt >> 48);

    asm("lgdt (%0)": :"p"(descriptor));

}


