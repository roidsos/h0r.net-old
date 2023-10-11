#ifndef SOUNDBLASTER16_H
#define SOUNDBLASTER16_H
#include <stdbool.h>

void testsb();
int sb_reset();
bool sb_init();
void sb_play(int freq);

#endif