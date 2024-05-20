#ifndef __BITMAP_H__
#define __BITMAP_H__

#include <libk/stdint.h>
#include <libk/stddef.h>
#include <libk/stdbool.h>

struct Bitmap{
    usize size;
    u8* buffer;
};

_bool bitmap_set(struct Bitmap bm,usize index,_bool setto);
_bool bitmap_get(struct Bitmap bm,usize index);

#endif // __BITMAP_H__