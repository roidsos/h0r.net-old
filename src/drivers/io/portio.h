#ifndef __PORTIO_H__
#define __PORTIO_H__
#include <stdint.h>

//outputs 8 bits TO THE IO BUS
void outb8(uint16_t port,uint8_t value);
//outputs 16 bits TO THE IO BUS
void outb16(uint16_t port,uint16_t value);
//outputs 32 bits TO THE IO BUS
void outb32(uint16_t port,uint32_t value);
//reads 8 bits TO THE IO BUS
uint8_t inb8(uint16_t port);
//reads 16 bits TO THE IO BUS
uint16_t inb16(uint16_t port);
//reads 32 bits TO THE IO BUS
uint32_t inb32(uint16_t port);
void iowait();

#endif // __PORTIO_H__