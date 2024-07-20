#ifndef __STDDEF_H__
#define __STDDEF_H__

#include "stdint.h"
#include "config.h"

#define NULL ((void *)0)

#define offsetof(type, member) ((usize)&((type *)0)->member)

typedef unsigned long usize;
typedef unsigned long size_t;

#if CHAR_WIDTH == 8
typedef u8 h0r_char;
#elif CHAR_WIDTH == 16
typedef u16 h0r_char;
#elif CHAR_WIDTH == 32
typedef u32 h0r_char;
#elif CHAR_WIDTH == 64
typedef u64 h0r_char;
#endif

#endif // __STDDEF_H__