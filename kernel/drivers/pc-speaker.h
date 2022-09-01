 #include <io.h>
 #include <stdint.h>
 class pcspeaker{
     public:
 //Play sound using built in speaker
 static void play_sound(uint_32 nFrequence);
 
 //make it shutup
 static void nosound();
 
 //Make a beep
 void beep();
 };
