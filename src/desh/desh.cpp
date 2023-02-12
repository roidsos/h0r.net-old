#include <desh/desh.h>
#include <io/io.h>
#include <lib/printf.h>
#include <drivers/keyboard.h>


char typedstring[255];
char idx = 0;
void DeshInit()
{
    printf(">");
}

void DeshUpdate()
{
    getstr(typedstring,255);
    if (typedstring == "version"){
        printf("DEfault SHell v0.00000000000000001 alpha\n");
    }
    else{
        printf("\n");
    }
    printf(">");
    
}
