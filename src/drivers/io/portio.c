#include "drivers/io/portio.h"

//output functions

void outb8(uint_16 port, uint_8 value) {
    asm("outb %1, %0" : : "dN" (port), "a" (value));
}

void outb16(uint_16 port, uint_16 value) {
    asm("outw %1, %0" : : "dN" (port), "a" (value));
}

void outb32(uint_16 port, uint_32 value) {
    asm("outl %1, %0" : : "dN" (port), "a" (value));
}

//input functions

uint_8 inb8(uint_16 port) {
    uint_8 r;
    asm("inb %1, %0" : "=a" (r) : "dN" (port));
    return r;
}

uint_16 inb16(uint_16 port){
    uint_16 r;
    asm("inw %1, %0" : "=a" (r) : "dN" (port));
    return r;
}

uint_32 inb32(uint_16 port){
    uint_32 r;
    asm("inl %1, %0" : "=a" (r) : "dN"(port));
    return r;
}