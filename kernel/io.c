#include "io.h"
int iter = 0;
uint_16 CursorPos;
void outb(uint_16 port,uint_8 value){
    asm volatile ("outb %0, %1" : :"a"(value) ,"Nd"(port));
}
uint_8 intb(uint_16 port){
    uint_8 ret;
    asm volatile ("inb %1, %0" : :"a"(ret) ,"Nd"(port));
    return ret;
}
void setCursorpos(uint_16 pos){
outb(0x3d4,0x0f);
outb(0x3d5,(uint_8)(pos & 0xff));
outb(0x3d4,0x0e);
outb(0x3d5,(uint_8)((pos >> 8) & 0xff));
CursorPos = pos;
}
void print(const int_8* str)
{
    static uint_16* VideoMemory = (uint_16*)0xb8000;
    int i = 0;
    int iter = CursorPos;
    while(str[i] != '\0'){
        VideoMemory[iter] = (VideoMemory[iter] & 0xFF00) | str[i];
        iter++;
        i++;
    }   
     setCursorpos(iter); 
}
void printc(const int_8* str,uint_8 color)
{
    static uint_16* VideoMemory = (uint_16*)0xb8000;
    int i = 0;
    int iter = CursorPos;
    while(str[i] != '\0'){
        VideoMemory[iter] = (VideoMemory[iter] & (color << 8)) | str[i];
        iter++;
        i++;
    }
    setCursorpos(iter);   
}