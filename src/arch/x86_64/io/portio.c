#include "portio.h"
#define PORT_UNUSED 0x80

void iowait() { outb8(PORT_UNUSED, 0); }

// output functions

// outputs 8 bits TO THE IO BUS
void outb8(uint16_t port, uint8_t value) {
    __asm__ volatile("outb %1, %0" : : "dN"(port), "a"(value));
}
// outputs 16 bits TO THE IO BUS
void outb16(uint16_t port, uint16_t value) {
    __asm__ volatile("outw %1, %0" : : "dN"(port), "a"(value));
}
// outputs 32 bits TO THE IO BUS
void outb32(uint16_t port, uint32_t value) {
    __asm__ volatile("outl %1, %0" : : "dN"(port), "a"(value));
}

// input functions

// reads 8 bits TO THE IO BUS
uint8_t inb8(uint16_t port) {
    uint8_t r;
    __asm__ volatile("inb %1, %0" : "=a"(r) : "dN"(port));
    return r;
}
// reads 16 bits TO THE IO BUS
uint16_t inb16(uint16_t port) {
    uint16_t r;
    __asm__ volatile("inw %1, %0" : "=a"(r) : "dN"(port));
    return r;
}
// reads 32 bits TO THE IO BUS
uint32_t inb32(uint16_t port) {
    uint32_t r;
    __asm__ volatile("inl %1, %0" : "=a"(r) : "dN"(port));
    return r;
}
