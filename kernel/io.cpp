#include <stdint.h>
int_32 lol = 0;
int iter = 0;
uint_16 CursorPos;
void outb8(uint_16 port,uint_8 value){
    asm volatile ("outb %0, %1" : :"a"(value) ,"Nd"(port));
}
void outb16(uint_16 port,uint_16 value){
    asm volatile ("outw %0, %1" : :"a"(value) ,"Nd"(port));
}
void outb32(uint_16 port,uint_32 value){
    asm volatile ("outl %0, %1" : :"a"(value) ,"Nd"(port));
}
void outbslow8(uint_16 port,uint_8 value){
     __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (value), "Nd" (port));
}

uint_8 inb8(uint_16 port){
    uint_8 ret;
    asm volatile ("inb %1, %0" : :"a"(ret) ,"Nd"(port));
    return ret;
}
uint_16 inb16(uint_16 port){
    uint_16 ret;
    asm volatile ("inw %1, %0" : :"a"(ret) ,"Nd"(port));
    return ret;
}
uint_32 inb32(uint_16 port){
    uint_32 ret;
    asm volatile ("inl %1, %0" : :"a"(ret) ,"Nd"(port));
    return ret;
}
void setCursorpos(uint_16 pos){
outb8(0x3d4,0x0f);
outb8(0x3d5,(uint_8)(pos & 0xff));
outb8(0x3d4,0x0e);
outb8(0x3d5,(uint_8)((pos >> 8) & 0xff));
CursorPos = pos;
}
void print(const int_8* str,uint_8 color)
{
    static uint_16* VideoMemory = (uint_16*)0xb8000;
    int i = 0;
    int iter = CursorPos;
    while(str[i] != '\0'){
        switch (str[i])
        {
        case 10://newline
            iter += 80;
            iter -= iter % 80;//automaticly returns on newlines like unix
            break;
        case 13://return
            iter -= iter % 80;
            break;
        
        default:
            VideoMemory[iter] = (VideoMemory[iter] & (color << 8)) | str[i];
            iter++;
        }
        
        i++;
    }
    setCursorpos(iter);   
}
void Clearscr(uint_8 color){
uint_64 value = 0;
value += (uint_64)color << 8;
value += (uint_64)color << 24;
value += (uint_64)color << 40;
value += (uint_64)color << 56;
    for(uint_64* i = (uint_64*)0xb8000;i < (uint_64*)0xb8000 + 4000;i++){
        *i = value;
    }
    setCursorpos(0);   
}
int_8 hex2strout[128];
const char* hex2str(uint_64 value){
    uint_8 size = 0;
    uint_64 sizetest = value;
    hex2strout[0] = '0';
    hex2strout[1] = 'x';

    while(sizetest / 16 > 0){
        sizetest /= 16;
        size++;
    }
    uint_8 index = 0;
    uint_64 newvalue = value;
    while(newvalue / 16 > 0){
        uint_8 remainder = newvalue % 16;
        newvalue /= 16;
        hex2strout[size-(index-2)] = remainder + (remainder > 9 ? 55 :48);
        index++;
    }
    uint_8 remainder = newvalue % 16;
    hex2strout[size - (index-2)] = remainder + (remainder > 9 ? 55 :48);
    hex2strout[size + 3] = 0;
    return hex2strout;
}
int_8 int2strout[128];
const char* int2str(uint_64 value){

    uint_8 size = 0;
    uint_64 sizetest = value;

    while(sizetest / 10 > 0){
        sizetest /= 10;
        size++;
    }
    uint_8 index = 0;
    uint_64 newvalue = value;
    while(newvalue / 10 > 0){
        uint_8 remainder = newvalue % 10;
        newvalue /= 10;
        int2strout[size-index] = remainder + 48;
        index++;
    }
    uint_8 remainder = newvalue % 10;
    int2strout[size-index] = remainder + 48;
    int2strout[size + 1] = 0;
    return int2strout;
}