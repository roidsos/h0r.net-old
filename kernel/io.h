#ifndef IO_HEADER
#define IO_HEADER
#include <stdint.h>
void print(const int_8* str,uint_8 color = 0x0f);
void Clearscr(uint_8 color = 0x0f);
void setCursorpos(uint_16 pos);
void outb8(uint_16 port,uint_8 value);
void outb16(uint_16 port,uint_16 value);
void outb32(uint_16 port,uint_32 value);
void outbslow8(uint_16 port,uint_8 value);
uint_8 inb8(uint_16 port);
uint_16 inb16(uint_16 port);
uint_32 inb32(uint_16 port);
const char* hex2str(uint_64 value);
const char* int2str(uint_64 value);
#endif 

