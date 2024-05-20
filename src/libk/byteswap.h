/* Macros and inline functions to swap the order of bytes in integer values.
   Copyright (C) 1997-2024 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#ifndef _BITS_BYTESWAP_H
#define _BITS_BYTESWAP_H 1

#include "stdint.h"

/* Swap bytes in 16-bit value.  */
#define __bswap_16(x)					\
  ((u16) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8)))


/* Swap bytes in 32-bit value.  */
#define __bswap_32(x)					\
  ((((x) & 0xff000000u) >> 24) | (((x) & 0x00ff0000u) >> 8)	\
   | (((x) & 0x0000ff00u) << 8) | (((x) & 0x000000ffu) << 24))

/* Swap bytes in 64-bit value.  */
#define __bswap_64(x)			\
  ((((x) & 0xff00000000000000ull) >> 56)	\
   | (((x) & 0x00ff000000000000ull) >> 40)	\
   | (((x) & 0x0000ff0000000000ull) >> 24)	\
   | (((x) & 0x000000ff00000000ull) >> 8)	\
   | (((x) & 0x00000000ff000000ull) << 8)	\
   | (((x) & 0x0000000000ff0000ull) << 24)	\
   | (((x) & 0x000000000000ff00ull) << 40)	\
   | (((x) & 0x00000000000000ffull) << 56))

static __inline u16
__uint16_identity (u16 __x)
{
  return __x;
}

static __inline u32
__uint32_identity (u32 __x)
{
  return __x;
}

static __inline u64
__uint64_identity (u64 __x)
{
  return __x;
}

#endif /* _BITS_BYTESWAP_H */