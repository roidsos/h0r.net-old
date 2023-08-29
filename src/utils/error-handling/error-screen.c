#include <vbe/font_renderer.h>
#include <arch/x86_64/essential.h>
#include "error-screen.h"
void trigger_error(int code,char* details)
{
    ClearScreen(0x00FFFF00);
    centeredStringRenderer("Error:",0,2,0x00AA0000);
    centeredStringRenderer(Hornet_error_codes[code],30,2,0x00AA0000);
    centeredStringRenderer(details,60,2,0x00AA0000);

    hcf();
}
