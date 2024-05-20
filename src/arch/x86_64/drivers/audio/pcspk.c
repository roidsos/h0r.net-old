#include "pcspk.h"

// TODO: PWM https://forum.osdev.org/viewtopic.php?f=13&t=17293 :meme:
void play_sound(u32 frequency) {
    u32 div;
    u8 tmp;

    div = 1193180 / frequency;
    outb8(0x43, 0xb6);
    outb8(0x42, (u8)(div));
    outb8(0x42, (u8)(div >> 8));

    // And play the sound using the PC speaker
    tmp = inb8(0x61);
    if (tmp != (tmp | 3)) {
        outb8(0x61, tmp | 3);
    }
}

// make it shut up
void nosound() {
    u8 tmp = inb8(0x61) & 0xFC;

    outb8(0x61, tmp);
}
