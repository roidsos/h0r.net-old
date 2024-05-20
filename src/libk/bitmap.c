#include "bitmap.h"

// Bit Magic: dont touch unless you have 2000000 IQ
// it took me half an hour in godbolt.org to figure out

_bool bitmap_set(struct Bitmap bm, usize index, _bool setto) {
    if (index > bm.size * 8)
        return false;
    register usize byteIndex = index / 8;
    register u8 bitIndex = index % 8;

    register u8 bitMask = (1 << (bitIndex));

    register u8 temp = bm.buffer[byteIndex] ^ (bm.buffer[byteIndex] & bitMask);

    bm.buffer[byteIndex] = temp | (setto << (bitIndex));
    return true;
}

_bool bitmap_get(struct Bitmap bm, usize index) {
    register usize byteIndex = index / 8;
    register u8 bitIndex = index % 8;

    register u8 bitIndexer = 1 << bitIndex;
    return bm.buffer[byteIndex] & bitIndexer;
}
