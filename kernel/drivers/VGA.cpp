#include <drivers/VGA.h>
bool VGA::SupportMode(uint_32 width, uint_32 height,uint_32 colors)
{
    return width == 320 && height == 200 && colors == 8;
}

bool VGA::SetMode(uint_32 width, uint_32 height,uint_32 colors)
{
    if(!SupportMode(width,height,colors)) return false;
    unsigned char g_320x200x256[] =
    {
        /* MISC */
        	0x63,
        /* SEQ */
        	0x03, 0x01, 0x0F, 0x00, 0x0E,
        /* CRTC */
        	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
        	0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        	0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
        	0xFF,
        /* GC */
        	0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
        	0xFF,
        /* AC */
        	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        	0x41, 0x00, 0x0F, 0x00,	0x00
    };
    unsigned char g_640x480x16[] =
    {
    /* MISC */
        0xE3,
    /* SEQ */
        0x03, 0x01, 0x08, 0x00, 0x06,
    /* CRTC */
        0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0B, 0x3E,
        0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xEA, 0x0C, 0xDF, 0x28, 0x00, 0xE7, 0x04, 0xE3,
        0xFF,
    /* GC */
        0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x05, 0x0F,
        0xFF,
    /* AC */
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
        0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
        0x01, 0x00, 0x0F, 0x00, 0x00
    };
    WriteReg(g_320x200x256);
    //WriteReg(g_640x480x16);
    return true;
}

void VGA::PutPixel(uint_32 x, uint_32 y, uint_8 r, uint_8 g, uint_8 b)
{
    PutPixel(y, x, GetCol(r, g, b));
}

uint_8* VGA::GetSeg()
{
    outb8(GCindex,0x06);
    uint_8 segmentnum = ((inb8(GCdata) >> 2) & 0x03);
    print(hex2str(segmentnum));
    return (uint_8*)0xA0000;
    switch(segmentnum){
        default:
        case 0:return (uint_8*)0x00000;
        case 1:return (uint_8*)0xA0000;
        case 2:return (uint_8*)0xB0000;
        case 3:return (uint_8*)0xB8000;
    }
}

void VGA::PutPixel(uint_32 x,uint_32 y,uint_8 color)
{

    //uint_16 index = ((640*y) + x)/8;//getting the index
    //uint_8* pixeladdr = 0xa0000;//getting the address of the pixel
    //
    //outb16(SEQindex,0x0102);//1.st bitplane
    //pixeladdr[index] = pixeladdr[index] | (((color << 7) >> index%8) & (0b10000000 >> index%8));
    //outb16(SEQindex,0x0202);//2.nd bitplane
    //pixeladdr[index] = pixeladdr[index] | (((color << 6) >> index%8) & (0b10000000 >> index%8));
    //outb16(SEQindex,0x0302);//3.rd bitplane
    //pixeladdr[index] = pixeladdr[index] | (((color << 5) >> index%8) & (0b10000000 >> index%8));
    //outb16(SEQindex,0x0402);//4.th bitplane
    //pixeladdr[index] = pixeladdr[index] | (((color << 4) >> index%8) & (0b10000000 >> index%8));
    //outb16(SEQindex,0x0f02);//f.th bitplane???
    //pixeladdr[(640*y)/8 + x/8] = *pixeladdr | ((color >> 4) & (0b00000001 << y%8));
     unsigned char* pixaddr = (unsigned char*)0xa0000;
     pixaddr[x+y*320] = color;
}

uint_8 VGA::GetCol(uint_32 r,uint_32 g,uint_8 b)
{
    if(r == 0 && g == 0 && b == 0xA8)
        return 1;
    return (uint_8)0;
}

void VGA::Clearscr(uint_8 color) {
    for(int y = 0; y < 480; y++) {
        for(int x = 0; x < 640; x++) {
            VGA::PutPixel(x, y, color);
        }
    }
}

void VGA::WriteReg(uint_8* reg)
{
    //misc
    outb8(miscport,*(reg++));
    //sequencer
    for(int i = 0; i < 5; i++){
        outb8(SEQindex,i);
        outb8(SEQdata,*(reg++));
    }
    //crtc
    outb8(CRTCindex,0x03);
    outb8(CRTCdata,inb8(CRTCdata) | 0x80);
    outb8(CRTCindex,0x11);
    outb8(CRTCdata,inb8(CRTCdata) & ~0x80);

    reg[0x03] = reg[0x03] | 0x80;
    reg[0x11] = reg[0x11] & ~0x80;
    for(int i = 0; i < 25; i++){
        outb8(CRTCindex,i);
        outb8(CRTCdata,*(reg++));
    }
    //gc
    for(int i = 0; i < 9; i++){
        outb8(GCindex,i);
        outb8(GCdata,*(reg++));
    }
    //ac
    for(int i = 0; i < 21; i++){
        inb8(ACreset);
        outb8(ACindex,i);
        outb8(ACwrite,*(reg++));
    }
    inb8(ACreset);
    outb8(ACindex,0x20);
}
