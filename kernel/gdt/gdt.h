#ifndef GDT_HEADER
#define GDT_HEADER
#include "stdint.h"

class GDT{
public:
    class Segment{

        uint_16 limit_low;
        uint_16 base_low;
        uint_8 base_hi;
        uint_8 type;
        uint_8 flags_limit_hi;
        uint_8 base_vhi;
    public:
        Segment(uint_32 base,uint_32 limit,uint_8 type);
        uint_32 GetBase();
        uint_32 GetLimit();

    }__attribute__((packed));  
    Segment nullseg;
    Segment unusedseg;
    Segment codeseg;
    Segment dataseg;
    GDT();
    ~GDT();
    uint_16 GetCodeseg();
    uint_16 GetDataseg(); 
};

#endif
