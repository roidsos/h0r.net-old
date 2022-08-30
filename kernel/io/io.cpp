#include <stdint.h>
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
// prints a string
void print(const int_8* str,uint_8 color)
{
    //set up some internal varibles
    static uint_16* VideoMemory = (uint_16*)0xb8000;
    int i = 0;
    int iter = CursorPos;
    //loops trough the string and outputs to the screen
    while(str[i] != '\0'){
        switch (str[i])
        {
        case 10://newline
            iter += 80;
            iter -= iter % 80;//automaticly returns on newlines like unix
            line_num += 1;
            break;
        case 13://return
            iter -= iter % 80;
            break;
        
        default:
            /*
            internal output format:
            1 byte - ASCII charachter
            1 byte - color data
            1 byte - ASCII charachter
            1 byte - color data
            and so on until the end of the buffer
            */
            VideoMemory[iter] = (VideoMemory[iter] & (color << 8)) | str[i];
            iter++;
        }
        vga_line_lengths[line_num] += iter;
        i++;
    }
    //adds the displayed string lenght to the cursor position
    setCursorpos(iter);   
}
//prints a char
void printchar(char chr, uint_8 color)
{
    // return if the char is nothing
    if (!chr) return; 
    
    //set up some internal varibles
    static uint_16* VideoMemory = (uint_16*)0xb8000;
    int iter = CursorPos;

    //outputs charachther the screen
    switch (chr)
    {
    case 10://newline
        iter += 80;
        iter -= iter % 80;//automaticly returns on newlines like unix
        line_num += 1;
        break;
    case 13://return
        iter -= iter % 80;
        break;
    
    default:
        VideoMemory[iter] = (VideoMemory[iter] & (color << 8)) | chr;
        iter++;
        vga_line_lengths[line_num]++;
    }
    setCursorpos(iter);   
}

//adds a backspace and moves the 
void backspace(){
    //set up some internal varibles
    static uint_16* VideoMemory = (uint_16*)0xb8000;
    register int iter = CursorPos;

    if(iter > 0){
        //decreese iter and delete the charachter right before the cursor
        iter--;
        vga_line_lengths[line_num]--;
        VideoMemory[iter] = ' ' | (VideoMemory[iter] & 0x0f00);
        setCursorpos(iter);
    }
    if(iter % 80 == 0 && iter != 0) {
        //go to the previous line and delete the charachter right before the cursor
        line_num--;
        iter -= 80 - vga_line_lengths[line_num];
        VideoMemory[iter] = ' ' | (VideoMemory[iter] & 0x0f00);
        setCursorpos(iter);
    }
}
//clears the screen with color!!
void Clearscr(uint_8 color){
//set up some internal varibles
uint_64 value = 0;
//create a 64-bit loop of color,0,color,0...
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
int_8 hex2strout[128];
//turns a number to a string(hex version)
const char* hex2str(uint_64 value){
    //set up some internal varibles
    uint_8 size = 0;
    uint_64 sizetest = value;
    //put "0x" at the beggining
    hex2strout[0] = '0';
    hex2strout[1] = 'x';
    //idk I forgot...
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
//turns a number to a string(dec version)
const char* int2str(uint_64 value){
    //set up some internal varibles
    uint_8 size = 0;
    uint_64 sizetest = value;
    //idk I forgot...
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

//enables text cursor trough hardware communication
void enable_text_cursor(uint_8 cursor_start, uint_8 cursor_end) {
	outb8(0x3D4, 0x0A);
	outb8(0x3D5, (inb8(0x3D5) & 0xC0) | cursor_start);
 
	outb8(0x3D4, 0x0B);
	outb8(0x3D5, (inb8(0x3D5) & 0xE0) | cursor_end);
}