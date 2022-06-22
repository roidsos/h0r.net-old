#include "gdt/gdt.h"
#include "io.h"
GDT::GDT():
nullseg(0,0,0),
unusedseg(0,0,0),
codeseg(0,64*1024*1024,0x9A),
dataseg(0,64*1024*1024,0x92)
{
    
    uint_16 i[5];
    i[0] = sizeof(GDT) - 1;
    i[1] = ((uint_64)this)& 0xffff;
    i[2] = ((uint_64)this>> 16)& 0xffff;
    i[3] = ((uint_64)this>>32)& 0xffff;
    i[4] = ((uint_64)this>>48)& 0xffff;
    
    asm volatile("lgdt (%0)" : :"p" (((uint_64*)i)));

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

GDT::Segment::Segment(uint_32 base,uint_32 limit,uint_8 type)
{
    uint_8* target = (uint_8*)this;

    if(limit <= 65536)
    {
        target[6] = 0x40;
    }
    else{
        if((limit & 0xfff) != 0xfff){
            limit = (limit >> 12)-1;
        }else{
            limit = limit >> 12;
        }
        target[6] = 0xC0;
    }
    target[0] = limit & 0xff;
    target[1] = (limit >> 8) & 0xff;
    target[6] |= (limit >> 16) & 0xf;

    target[2] = base & 0xff;
    target[3] = (base >> 8) & 0xff;
    target[4] = (base >> 16) & 0xff;
    target[7] = (base >> 24) & 0xff;

    target[5] = type;

}
