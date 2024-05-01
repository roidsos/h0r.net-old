#include "pcspk.h"

//TODO: PWM https://forum.osdev.org/viewtopic.php?f=13&t=17293 :meme:
void play_sound(uint32_t frequency) {
    uint32_t div;
    uint8_t tmp;

    div = 1193180 / frequency;
    outb8(0x43, 0xb6);
    outb8(0x42, (uint8_t) (div) );
    outb8(0x42, (uint8_t) (div >> 8));

    //And play the sound using the PC speaker
    tmp = inb8(0x61);
    if (tmp != (tmp | 3)) {
        outb8(0x61, tmp | 3);
    }
 }

 //make it shut up
void nosound() {
    uint8_t tmp = inb8(0x61) & 0xFC;

    outb8(0x61, tmp);
}
