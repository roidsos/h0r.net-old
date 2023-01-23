#include <desh/desh.h>
#include <io/io.h>
#include <lib/printf.h>
#include <drivers/memory/Heap.h>
#include <drivers/keyboard.h>
#include <drivers/PIT.h>

char typedstring[255];
char idx = 0;
void DeshInit()
{
    printf(">");
}

void DeshUpdate()
{
char _char = 0;
if (!keyboard_key(KEY_ENTER)){
    if (idx <= 254) 
        return;

    if (_char = getch()){
        printf("%n",_char);
        typedstring[idx] = _char;
        idx++;
    }
}else{
    typedstring[idx] = 0;
    idx = 0;
    if (typedstring == "version"){
        printf("DEfault SHell v0.00000000000000001 alpha\n");
    }
    else{
        printf("\n");
    }
    printf(">");
    
}
PIT::Sleep(30);
}
