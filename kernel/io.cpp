#include <stdint.h>
int_32 lol = 0;
int iter = 0;
uint_16 CursorPos;
void outb(uint_16 port,uint_8 value){
    asm volatile ("outb %0, %1" : :"a"(value) ,"Nd"(port));
}
void outbslow(uint_16 port,uint_8 value){
     __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (value), "Nd" (port));
}

uint_8 inb(uint_16 port){
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
    uint_64* valptr = &value;
    uint_8* ptr;
    uint_8 temp;
    uint_8 size = (sizeof(uint_64)) * 2 - 1;
    uint_8 i;
    for(i = 0;i < size;i++){
        ptr = ((uint_8*)valptr + i);
        temp = ((*ptr & 0xF0) >> 4);
        hex2strout[size - (i *2 + 1)] = temp + (temp > 9 ? 55 :48);
        temp = ((*ptr & 0x0F));
        hex2strout[size - (i *2 + 0)] = temp + (temp > 9 ? 55 :48);
    }
    hex2strout[size + 1] = 0;
    return hex2strout;
}