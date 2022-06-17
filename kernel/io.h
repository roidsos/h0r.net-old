#include <stdint.h>
void print(const int_8* str,uint_8 color = 0x0f);
void Clearscr(uint_8 color = 0x0f);
void setCursorpos(uint_16 pos);
void outb(uint_16 port,uint_8 value);
uint_8 intb(uint_16 port);
const char* hex2str(int value);