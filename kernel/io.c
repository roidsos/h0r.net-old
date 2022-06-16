int iter = 0;
void print(char* str)
{
    static unsigned short* VideoMemory = (unsigned short*)0xb8000;
    int i = 0;
    while(str[i] != '\0'){
        VideoMemory[iter] = (VideoMemory[iter] & 0xFF00) | str[i];
        iter++;
        i++;
    }
        
    
}