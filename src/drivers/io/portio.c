#include "drivers/io/portio.h"
#define UNUSED_PORT 0x80

void iowait() { outb8(UNUSED_PORT, 0); }

// output functions

void outb8(uint16_t port, uint8_t value) {
    asm("outb %1, %0" : : "dN"(port), "a"(value));
}

void outb16(uint16_t port, uint16_t value) {
    asm("outw %1, %0" : : "dN"(port), "a"(value));
}

void outb32(uint16_t port, uint32_t value) {
    asm("outl %1, %0" : : "dN"(port), "a"(value));
}

// input functions

uint8_t inb8(uint16_t port) {
    uint8_t r;
    asm("inb %1, %0" : "=a"(r) : "dN"(port));
    return r;
}

uint16_t inb16(uint16_t port) {
    uint16_t r;
    asm("inw %1, %0" : "=a"(r) : "dN"(port));
    return r;
}

uint32_t inb32(uint16_t port) {
    uint32_t r;
    asm("inl %1, %0" : "=a"(r) : "dN"(port));
    return r;
}