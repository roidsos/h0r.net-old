#include "PC-speaker.h"
#include "arch/x86/portion.h"
void play_sound(uint32_t nFrequence) {
 	uint_32 Div;
 	uint_8 tmp;
 
 //Set the PIT to the desired frequency
 	Div = 1193180 / nFrequence;
 	outb8(0x43, 0xb6);
 	outb8(0x42, (uint_8) (Div) );
 	outb8(0x42, (uint_8) (Div >> 8));
 
    //And play the sound using the PC speaker
 	tmp = inb8(0x61);
  	if (tmp != (tmp | 3)) {
 		outb8(0x61, tmp | 3);
 	}
 }
 
 //make it shut up
void nosound() {
 	uint_8 tmp = inb8(0x61) & 0xFC;
 
 	outb8(0x61, tmp);
 }
