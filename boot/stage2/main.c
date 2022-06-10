static unsigned short* VideoMemory = (unsigned short*)0xb8000;
void cstart(){
VideoMemory[1] = 'h';
}