#include "gdt.h"

__attribute__((aligned(0x1000)))
struct GDT DefaultGDT = {
    {0, 0, 0, 0b00000000, 0b00000000, 0}, // null
    {0, 0, 0, 0b10011010, 0b10100000, 0}, // kernel code segment
    {0, 0, 0, 0b10010010, 0b10100000, 0}, // kernel data segment
    {0, 0, 0, 0b00000000, 0b00000000, 0}, // user null
    {0, 0, 0, 0b11111010, 0b10100000, 0}, // user code segment
    {0, 0, 0, 0b11110010, 0b10100000, 0}, // user data segment
};
void LoadGDT(struct GDTDescriptor* gdtDescriptor);

void load_default_gdt(){
    struct GDTDescriptor gdtDescriptor;
    gdtDescriptor.Size = sizeof(struct GDT) - 1;
    gdtDescriptor.Offset = (uint64_t)&DefaultGDT;
    LoadGDT(&gdtDescriptor);
}