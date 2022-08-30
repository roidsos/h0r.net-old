#include "gdt/gdt.h"
#include "io/io.h"

//dont look here ,I never finished this shit
GDT::GDT()
{
    uint_64* ptr = (uint_64*)_gdt;
    ptr[0] = 0;
}

GDT::GDT32::GDT32()
{
    
}
