#include "snake.h"
#include "drivers/keyboard.h"
#include "memory/Heap.h"
#include "stdint.h"
namespace Snake
{
    VGA vga;
    char ass;
    uint_8 x,y;
    void Init()
    {
        //ass = malloc(1);
        vga.Clearscr(0x0);
        //x = 0;
        //y = 0;
    }
    
    void Update()
    {
        if(getch() == 'w'){
            y++;
            }
         if(getch() == 'a'){
            x--;        
            }
         if(getch() == 's'){
            y--;        
            }
         if(getch() == 'd'){
            x++;        
            }
    vga.Clearscr(0x0);
    vga.PutPixel(x,y,4);
        
    }
}