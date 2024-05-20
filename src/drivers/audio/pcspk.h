#ifndef PCSPK_H
#define PCSPK_H
#include <libk/stdtypes.h>
#include <arch/x86_64/io/portio.h>

//TODO: audio subsystem

void play_sound(uint32_t frequency);

void nosound();

#endif
