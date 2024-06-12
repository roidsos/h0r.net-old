#ifndef PS2_H
#define PS2_H

#include <libk/stdint.h>

#define PS2_COMMAND     0x64
#define PS2_DATA        0x60

extern char KB_STATE[2];
extern u32 KB_CAPS_STATE;
extern u32 KB_SHIFT_STATE;

void ps2_init();

#endif
