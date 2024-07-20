#ifndef __PORTIO_H__
#define __PORTIO_H__
#include <core/libk/stdint.h>

//outputs 8 bits TO THE IO BUS
void o8(u16 port,u8 value);
//outputs 16 bits TO THE IO BUS
void o16(u16 port,u16 value);
//outputs 32 bits TO THE IO BUS
void o32(u16 port,u32 value);
//reads 8 bits TO THE IO BUS
u8 in8(u16 port);
//reads 16 bits TO THE IO BUS
u16 in16(u16 port);
//reads 32 bits TO THE IO BUS
u32 in32(u16 port);
void iowait();

#endif // __PORTIO_H__