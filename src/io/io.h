#ifndef IO_HEADER
#define IO_HEADER
#include <util/stdint.h>

void IOWait();
void printchar(char chr,uint_8 color = 0xff);
void backspace();
void Clearscr(uint_8 color = 0x0f);
void setCursorpos(uint_16 pos);
void enable_text_cursor(uint_8 cursor_start, uint_8 cursor_end);
void outb8(uint_16 port,uint_8 value);
void outb16(uint_16 port,uint_16 value);
void outb32(uint_16 port,uint_32 value);
void outbslow8(uint_16 port,uint_8 value);
uint_8 inb8(uint_16 port);
uint_16 inb16(uint_16 port);
uint_32 inb32(uint_16 port);
#endif 

