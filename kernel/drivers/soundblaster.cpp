
// Code used: https://forum.osdev.org/viewtopic.php?f=1&t=32879
#include <io.h>
#include <stdint.h>

#define DSP_RESET 0x226
#define DSP_READ 0x22A
#define DSP_WRITE 0x22C
#define DSP_BUFFER 0x22E
#define DSP_INTERRUPT 0x22F



uint_8 sound_blaster=false;
uint_8 sb16_version_major=false;
uint_8 sb16_version_minor=false;

void reset_DSP(void) {
          outb8(DSP_RESET, 1);
          outb8(DSP_RESET, 0);

          if(inb8(DSP_READ)==0xAA) {
                sound_blaster=true;
          }
}

void write_DSP(int value) {
          uint_8 status=0;
          status=inb8(DSP_WRITE);
          outb8(DSP_WRITE, value);
}

int read_DSP(void) {
          return inb8(DSP_READ);
}

void init_SB16(void) {
          reset_DSP();

          if(sound_blaster==false) {
                    return;
          }

          write_DSP(0xE1);
          sb16_version_major=read_DSP();
          sb16_version_minor=read_DSP();

}