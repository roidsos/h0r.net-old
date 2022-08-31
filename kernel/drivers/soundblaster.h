#ifndef __SOUNDBLASTER_H__
#define __SOUNDBLASTER_H__

extern uint_8 sound_blaster;
extern uint_8 sb16_version_major;
extern uint_8 sb16_version_minor;

void enable_speaker();
void disable_speaker();

void set_volume(uint_8 volume);
void set_sample_rate(uint_8 srhi,uint_8 srlo);


void reset_DSP(void);
void write_DSP(int value);
int read_DSP(void);
void init_SB16(void);

#endif // __SOUNDBLASTER_H__