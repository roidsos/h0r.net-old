#include <drivers/pc-speaker.h>
#include <drivers/PIT.h>
 //Play sound using built in speaker
void pcspeaker::play_sound(uint_32 nFrequence) {
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
 
 //make it shutup
void pcspeaker::nosound() {
 	uint_8 tmp = inb8(0x61) & 0xFC;
 
 	outb8(0x61, tmp);
 }
 
 //Make a beep
 void pcspeaker::beep() {
 	 play_sound(1000);
 	 PIT::Sleep(10);
 	 nosound();
 }