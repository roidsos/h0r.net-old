#ifndef __BITMAP_H__
#define __BITMAP_H__

#include <core/libk/stdint.h>
#include <core/libk/stddef.h>
#include <core/libk/stdbool.h>

struct Bitmap{
    usize size;
    u8* buffer;
};

_bool bitmap_set(struct Bitmap bm,usize index,_bool setto);
_bool bitmap_get(struct Bitmap bm,usize index);

#endif // __BITMAP_H__