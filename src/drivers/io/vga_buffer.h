#ifndef __VGA_BUFFER_H__
#define __VGA_BUFFER_H__
#include "utils/types.h"

void cursorpos_update();
void cursorpos_set(short pos);

void clear(uint_8 color);

void print_char(char ch,char color);
#define printchar print_char

#endif // __VGA_BUFFER_H__