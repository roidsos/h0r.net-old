#include <stdint.h>
void print(const int_8* str);
void printc(const int_8* str,uint_8 color);
void setCursorpos(uint_16 pos);
void outb(uint_16 port,uint_8 value);
uint_8 intb(uint_16 port);