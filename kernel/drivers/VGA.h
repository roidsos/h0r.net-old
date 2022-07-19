#ifndef __VGA_H__
#define __VGA_H__
#include <stdint.h>
#include <io.h>

#define miscport  0x3c2
#define CRTCindex 0x3d4
#define CRTCdata  0x3d5
#define SEQindex  0x3c4
#define SEQdata   0x3c5
#define GCindex   0x3ce
#define GCdata    0x3cf
#define ACindex   0x3c0
#define ACread    0x3c1
#define ACwrite   0x3c0
#define ACreset   0x3da
class VGA{
  
void WriteReg(uint_8* reg);
uint_8* GetSeg();

void PutPixel(uint_32 x,uint_32 y,uint_8 color);
uint_8 GetCol(uint_32 r,uint_32 g,uint_8 b);

public:
    bool SupportMode(uint_32 width, uint_32 height,uint_32 colors);
    bool SetMode(uint_32 width, uint_32 height,uint_32 colors);
    void PutPixel(uint_32 x,uint_32 y,uint_8 r,uint_8 g,uint_8 b);
};


#endif // __VGA_H__