#ifndef __HWRAND_H__
#define __HWRAND_H__
#include <types/stdtypes.h>

#define HWRAND_SRC_IDC      0
#define HWRAND_SRC_RDSEED   1
#define HWRAND_SRC_RDRAND   2

uint64_t rand(uint8_t source);


#endif // __HWRAND_H__