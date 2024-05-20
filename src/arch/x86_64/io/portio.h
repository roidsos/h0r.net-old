#ifndef __PORTIO_H__
#define __PORTIO_H__
#include <libk/stdint.h>

//outputs 8 bits TO THE IO BUS
void outb8(u16 port,u8 value);
//outputs 16 bits TO THE IO BUS
void outb16(u16 port,u16 value);
//outputs 32 bits TO THE IO BUS
void outb32(u16 port,u32 value);
//reads 8 bits TO THE IO BUS
u8 inb8(u16 port);
//reads 16 bits TO THE IO BUS
u16 inb16(u16 port);
//reads 32 bits TO THE IO BUS
u32 inb32(u16 port);
void iowait();

#endif // __PORTIO_H__