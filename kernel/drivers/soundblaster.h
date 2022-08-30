#ifndef __SOUNDBLASTER_H__
#define __SOUNDBLASTER_H__

void reset_DSP(void);
void write_DSP(int value);
int read_DSP(void);
void init_SB16(void);

#endif // __SOUNDBLASTER_H__