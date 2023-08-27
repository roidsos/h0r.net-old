#ifndef __BITMAP_H__
#define __BITMAP_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

struct Bitmap{
    size_t size;
    uint8_t* buffer;
};

void bitmap_set(struct Bitmap bm,size_t index,bool setto);
bool bitmap_get(struct Bitmap bm,size_t index);

#endif // __BITMAP_H__