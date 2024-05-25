#ifndef PCSPK_H
#define PCSPK_H
#include <libk/stdint.h>
#include <arch/x86_64/io/portio.h>

//TODO: audio subsystem

void play_sound(u32 frequency);

void nosound();

#endif
