#ifndef GDT_HEADER
#define GDT_HEADER
#include "stdint.h"

class GDT{
    uint_8 fullness;
    struct GDT32{
        uint_16 limit_low;
        uint_16 base_low;
        uint_8  base_mid;
        uint_8  access;
        uint_8  limit_flags;
        uint_8  base_hi;
        GDT32();
    };
    GDT32 _gdt[256];
    public:
    GDT();
    void AddSegment();
    void Load();
};

#endif