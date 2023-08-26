#ifndef GDT_H
#define GDT_H
#include <stdint.h>

struct GDTDescriptor {
    uint16_t Size;
    uint64_t Offset;
} __attribute__((packed));

struct GDTEntry {
    uint16_t Limit0;
    uint16_t Base0;
    uint8_t Base1;
    uint8_t AccessByte;
    uint8_t Limit1_Flags;
    uint8_t Base2;
}__attribute__((packed));

struct GDT {
    struct GDTEntry Null; //0x00
    struct GDTEntry KernelCode; //0x08
    struct GDTEntry KernelData; //0x10
    struct GDTEntry UserNull;
    struct GDTEntry UserCode;
    struct GDTEntry UserData;
} __attribute__((packed)) 
__attribute((aligned(0x1000)));

extern struct GDT DefaultGDT;

void LoadGDT(struct GDTDescriptor* gdtDescriptor);
#endif
