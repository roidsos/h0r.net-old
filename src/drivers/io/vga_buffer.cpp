#include "utils/colors.h"
#include "drivers/io/portio.h"

#define BUFFER_HEIGHT  25
#define BUFFER_WIDTH   80
static uint_16* VideoMemory = (uint_16*)0xb8000;

short Cursorpos = 0;

void cursorpos_update(){
    outb8(0x3d4,0x0f);
    outb8(0x3d5,(uint_8)(Cursorpos & 0xff));
    outb8(0x3d4,0x0e);
    outb8(0x3d5,(uint_8)((Cursorpos >> 8) & 0xff));
}

void cursorpos_set(short pos){
    Cursorpos = pos;
    cursorpos_update();
}
char process_char(char ch){
    if(ch == 10){ // use yoda syntax (jk)
        Cursorpos += 80;
        Cursorpos -= Cursorpos % 80;//automaticly returns on newlines like unix
        return 0;
    }else if(ch == 13){
        Cursorpos -= Cursorpos % 80;//gotta support windows line endings
        return 0;
    }else{
        return ch;
    }
}

void print_char(char ch,char color){
    char ch_processed = process_char(ch);

    if (ch_processed){
        VideoMemory[Cursorpos] = (color << 8) | ch;
        Cursorpos++;
        cursorpos_update();
    }
}
void clear(uint_8 color){

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
cursorpos_set(0);
}