#include <drivers/PIT.h>
#include <gdt/gdt.h>
#include <interrupts/IDT.h>
#include <io.h>
#include <memory/Heap.h>
#include <memory/memory.h>
#include <render/font_render.h>

//#include <drivers/ata.h>
#include "colors.h"
#include <drivers/VGA.h>
#include <drivers/keyboard.h>
#include <drivers/pc-speaker.h>
#include <render/renderer.h>
#include <powermanagement.h>
#include <windowmanager.h>

void *keybuffer;

inline const char *const BoolToString(bool b) { return b ? "true" : "false"; }
extern "C" int kernel_main()
{
  pcspeaker sp;

  InitHeap(0x100000, 0x100000); // initialize the heap

  InitIDT(); // initialize the IDT

  keybuffer = calloc(2);

  VGA vga;
  // vga.SetMode(320, 200, 8);
  Clearscr(0xF);
  // enable_text_cursor(14, 15);

<<<<<<< HEAD
  // static uint_16 *pixel = (uint_16 *)0xa0000;
  // for (int i = 0; i < 320 * 200; i++)
  // {
  //   pixel[i] = 0xF;
  // }
  // unsigned char *VGAPIXELS = 0xa0000;
  // for (int i = 0; i < 320 * 200; i++)
  // {
  //   VGAPIXELS[i] = 0x0;
  // }
// 
  // char str[]{55, // W
  //            69, // e
  //            76, // l
  //            67, // c
  //            79, // o
  //            77, // m
  //            69, // e
  //            96, //
  //            69, // e
  //            0};
  //
  PIT::PitInit();
  PIT::Sleep(10);
  // CreateWin(0, 10, 10, 250, 150, "Test", 0xF, WHITE);
  while (1)
  {
    if (keyboard_char('r') && keyboard_key(KEY_LCTRL))
    {
      reboot();
=======
    Clearscr(0x0F);


    //VGA vga;
    //vga.SetMode(320, 200, 8);

    //Snake::Init();
    enable_text_cursor(14, 15);
   print("Type here but please dont delete all cuz it bugs out like hell!!!");

    // static uint_16* pixel = (uint_16*)0xa0000;

    // for (int i = 0; i < 320 * 200; i++) {
    //     pixel[i] = 0x7;
    // }
    // unsigned char* VGAPIXELS = 0xa0000;
    // for (int i = 0; i < 320 * 200; i++) {
    //     VGAPIXELS[i] = 0x7;
    // }
    // *pixel = 0xE;
    // pixel++;
    // *pixel = 0xE;
    // *pixel++ = 0x2;
    // pixel++;
    // ++pixel;
    // ++*pixel = 0x2;
    // *pixel++ = 0x2;
    char str[] {
        55,//W
        69,//e
        76,//l
        67,//c
        79,//o
        77,//m
        69,//e
        96,// 
        69,//e
        0
    };






    while(1){
        //Snake::Update();
        // PIT::Sleep(1000);
        // print(hex2str((uint_64)PIT::()));
        // print("       ");
        // setCursorpos(0);
        //sp.beep();
>>>>>>> e744f35edafaa133699ff81ade0737e057375cae
    }
    else if (keyboard_char(KEY_LCTRL) && keyboard_key(KEY_LCTRL))
    {
      
    }
  }
}