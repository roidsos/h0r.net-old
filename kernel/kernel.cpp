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
    }
    else if (keyboard_char(KEY_LCTRL) && keyboard_key(KEY_LCTRL))
    {
      
    }
  }
}