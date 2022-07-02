#include "gdt/gdt.h"
#include "io.h"
GDT::GDT():
nullseg(0,0,0),
codeseg(0,64*1024*1024,0b10011010),
dataseg(0,64*1024*1024,0b10010010)
{
    
    GDT_pointer gdt;
    gdt.size =sizeof(GDT) - 1;
    gdt.base = (uint_64)this;
    
    
    asm volatile("lgdt (%0)" : :"p" (&gdt));

}

GDT::~GDT()
{
    
}

uint_16 GDT::GetCodeseg()
{
    return (uint_8*)&codeseg - (uint_8*)this;

}

uint_16 GDT::GetDataseg()
{
    return (uint_8*)&dataseg - (uint_8*)this;
}

uint_32 GDT::Segment::GetLimit()
{
uint_8* target = (uint_8*)this;
uint_32 result = target[6] & 0xf;
result = (result << 8) + target[1];
result = (result << 8) + target[0];
if((target[6] & 0xc0) == 0xc0)
    result = (result << 12) & 0x12;
return result;
}

uint_32 GDT::Segment::GetBase()
{
uint_8* target = (uint_8*)this;
uint_32 result = target[7];
result = (result << 8) + target[4];
result = (result << 8) + target[3];
result = (result << 8) + target[2];
return result;
}

GDT::Segment::Segment(uint_32 base,uint_32 limit,uint_8 flags)
{

    if(limit <= 65536)
    {
        flags_limit_hi = 0b01100000;
    }
    else{
        if((limit & 0xfff) != 0xfff){
            limit = (limit >> 12)-1;
        }else{
            limit = limit >> 12;
        }
        flags_limit_hi = 0b11100000;
    }
    limit_low       = limit & 0xffff;
    flags_limit_hi |= (limit >> 16) & 0xff;

    base_low    = base & 0xffff;
    base_hi     = (base >> 16) & 0xff;
    base_vhi    = (base >> 24) & 0xff;

    type = flags;

}
