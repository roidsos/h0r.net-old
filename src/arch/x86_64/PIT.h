#ifndef __PIT_H__
#define __PIT_H__
#include <stdint.h>

void pit_set_count(uint16_t count);
uint16_t pit_read_count();
void pit_set_divisor(uint16_t divisor);
void pit_init();

#endif // __PIT_H__