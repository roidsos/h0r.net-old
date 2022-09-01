#ifndef __IDT_H__
#define __IDT_H__
#include <stdint.h>
#include <io.h>
#include <gdt/gdt.h>


struct IDT64
{
    uint_16 offset_low;
    uint_16 selector;
    uint_8  ist;
    uint_8  types_attr;
    uint_16 offset_mid;
    uint_32 offset_high;
    uint_32 zero;
} __attribute__ ((packed));

extern "C" IDT64 _idt[256];

void InitIDT();

void AddEntry(void* Isr,int attr,int _num);

#endif