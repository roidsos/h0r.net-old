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
#include <drivers/soundblaster.h>
#include <render/renderer.h>
#include <powermanagement.h>
#include <windowmanager.h>

void *keybuffer;

inline const char *const BoolToString(bool b) { return b ? "true" : "false"; }
extern "C" int kernel_main()
{
  // pcspeaker sp;

  InitHeap(0x100000, 0x100000); // initialize the heap

  InitIDT(); // initialize the IDT

  keybuffer = calloc(2);

  VGA vga;
  vga.SetMode(320, 200, 8);
  Clearscr(0x0);

  // CreateWin(0, 10, 10, 250, 150, "Test", 0xF, WHITE);

  // Counter
  // int counter = 0;
  renderString(20, 20, 100, 0xF, "HORNET");


  init_SB16();
  reset_DSP();
  write_DSP(0xC);

  // varj megcsinalom a qemut
  UpdateAndRender();
  while (1)
  {
    if (keyboard_char('r') && keyboard_key(KEY_LCTRL))
    {
      reboot();
    }
    else if (keyboard_key(KEY_LCTRL) && keyboard_key(KEY_ENTER))
    {
      CreateWin(1, 100, 100, 100, 100, "DEFAULT_WINDOW", 0xF, WHITE);
      UpdateAndRender();
    }
    else if (keyboard_key(KEY_LCTRL) && keyboard_char('w'))
    {
      DeleteWin(0);
    }
    // vga.Clearscr(0);
    // UpdateAndRender(); // update the f***ing screen
  }
}