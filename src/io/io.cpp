#include <util/stdint.h>
#include <drivers/memory/memory.h>


int iter = 0;
int line_num = 0;
int vga_line_lengths[24] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint_16 CursorPos;
//wait function
void IOWait(){
    asm volatile ("outb %%al, $0x80" : : "a"(0));
}

//output functions

void outb8(uint_16 port, uint_8 value) {
    asm("outb %1, %0" : : "dN" (port), "a" (value));
}

void outb16(uint_16 port, uint_16 value) {
    asm("outw %1, %0" : : "dN" (port), "a" (value));
}

void outb32(uint_16 port, uint_32 value) {
    asm("outl %1, %0" : : "dN" (port), "a" (value));
}

void outbslow8(uint_16 port,uint_8 value){
     __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (value), "Nd" (port));
}

//input functions

uint_8 inb8(uint_16 port) {
    uint_8 r;
    asm("inb %1, %0" : "=a" (r) : "dN" (port));
    return r;
}

uint_16 inb16(uint_16 port){
    uint_16 r;
    asm("inw %1, %0" : "=a" (r) : "dN" (port));
    return r;
}

uint_32 inb32(uint_16 port){
    uint_32 r;
    asm("inl %1, %0" : "=a" (r) : "dN"(port));
    return r;
}

// sets the cursor pos trough hardware communication
void setCursorpos(uint_16 pos){
outb8(0x3d4,0x0f);
outb8(0x3d5,(uint_8)(pos & 0xff));
outb8(0x3d4,0x0e);
outb8(0x3d5,(uint_8)((pos >> 8) & 0xff));
CursorPos = pos;
}
 
//prints a char
void printchar(char chr, uint_8 color)
{
    // return if the char is nothing
    if (!chr) return; 
    
    //set up some internal varibles
    static uint_16* VideoMemory = (uint_16*)0xb8000;
    //outputs charachther the screen
    switch (chr)
    {
    case 10://newline
        CursorPos += 80;
        CursorPos -= CursorPos % 80;//automaticly returns on newlines like unix
        break;
    case 13://return
        CursorPos -= CursorPos % 80;
        break;
    
    default:

        if(CursorPos > 80*22){
            memcpy(VideoMemory,VideoMemory + 80,80*40);
            memset(VideoMemory + 80*23,0,80 * 3);  
            CursorPos = 80*21;
        }

        VideoMemory[CursorPos] = (VideoMemory[CursorPos] & (color << 8)) | chr;
        CursorPos++;

        line_num = CursorPos / 80;
        vga_line_lengths[line_num]++;
    }

    setCursorpos(CursorPos);   
    
}

//adds a backspace and moves the cursor
void backspace(){
    static uint_16* VideoMemory = (uint_16*)0xb8000;

    if(CursorPos > 0){

        if(CursorPos % 80 == 0) {
            line_num--;
            CursorPos -= 80 - vga_line_lengths[line_num];
            goto finish;
        }

        CursorPos--;
        vga_line_lengths[line_num]--;

        finish:
        VideoMemory[CursorPos] = ' ' | (VideoMemory[CursorPos] & 0x0f00);
        setCursorpos(CursorPos);
    }
    
}
//clears the screen with color!!
void Clearscr(uint_8 color){

//create a 64-bit loop of color,0,color,0...
uint_64 value = 0;
value += (uint_64)color << 8;
value += (uint_64)color << 24;
value += (uint_64)color << 40;
value += (uint_64)color << 56;
// fill the screen with it
for(uint_64* i = (uint_64*)0xb8000;i < (uint_64*)0xb8000 + 4000;i++){
    *i = value;
}
//set the cursor position to the top right
setCursorpos(0);
}

//enables text cursor trough hardware communication
void enable_text_cursor(uint_8 cursor_start, uint_8 cursor_end) {
	outb8(0x3D4, 0x0A);
	outb8(0x3D5, (inb8(0x3D5) & 0xC0) | cursor_start);
 
	outb8(0x3D4, 0x0B);
	outb8(0x3D5, (inb8(0x3D5) & 0xE0) | cursor_end);
}