#ifndef IO_HEADER
#define IO_HEADER
#include <stdint.h>
void print(const int_8* str,uint_8 color = 0x0f);
void Clearscr(uint_8 color = 0x0f);
void setCursorpos(uint_16 pos);
void outb(uint_16 port,uint_8 value);
uint_8 inb(uint_16 port);
template<typename T>
const char* hex2str(T value);
#endif 

