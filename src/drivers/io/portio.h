#ifndef __PORTIO_H__
#define __PORTIO_H__
#include "utils/types.h"

void outb8(uint_16 port,uint_8 value);
void outb16(uint_16 port,uint_16 value);
void outb32(uint_16 port,uint_32 value);
uint_8 inb8(uint_16 port);
uint_16 inb16(uint_16 port);
uint_32 inb32(uint_16 port);

#endif // __PORTIO_H__