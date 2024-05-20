#include "portio.h"
#define PORT_UNUSED 0x80

void iowait() { outb8(PORT_UNUSED, 0); }

// output functions

// outputs 8 bits TO THE IO BUS
void outb8(u16 port, u8 value) {
    __asm__ volatile("outb %1, %0" : : "dN"(port), "a"(value));
}
// outputs 16 bits TO THE IO BUS
void outb16(u16 port, u16 value) {
    __asm__ volatile("outw %1, %0" : : "dN"(port), "a"(value));
}
// outputs 32 bits TO THE IO BUS
void outb32(u16 port, u32 value) {
    __asm__ volatile("outl %1, %0" : : "dN"(port), "a"(value));
}

// input functions

// reads 8 bits TO THE IO BUS
u8 inb8(u16 port) {
    u8 r;
    __asm__ volatile("inb %1, %0" : "=a"(r) : "dN"(port));
    return r;
}
// reads 16 bits TO THE IO BUS
u16 inb16(u16 port) {
    u16 r;
    __asm__ volatile("inw %1, %0" : "=a"(r) : "dN"(port));
    return r;
}
// reads 32 bits TO THE IO BUS
u32 inb32(u16 port) {
    u32 r;
    __asm__ volatile("inl %1, %0" : "=a"(r) : "dN"(port));
    return r;
}
