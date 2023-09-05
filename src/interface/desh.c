#include "desh.h"
#include <arch/x86_64/essential.h>
#include <arch/x86_64/power.h>
#include <drivers/hid/keyboard.h>
#include <drivers/io/pci.h>
#include <utils/string.h>
#include <vbe/font_renderer.h>
#include <vendor/printf.h>

char typedstring[255];
char idx = 0;
void DeshInit() { printf(">"); }

void parseCommand(char *command) {
  char **args = split(command, ' ');
  printf_("\n");
  if (strcmp(args[0], "version")) {
    printf_("DEfault SHell v0.00000000000000001 alpha\n");
  } else if (strcmp(args[0], "reboot")) {
    sys_reboot();
  } else if (strcmp(args[0], "clear") || strcmp(args[0], "cls")) {
    ClearScreen(0x00000000);
  } else if (strcmp(args[0], "echo")) {
    for (size_t i = 1; args[i] != 0; i++) {
      printf_("%s", args[i]);
    }
    printf_("\n");
  } else if (strcmp(args[0], "k")) // shortened it to k so if I try to type the
                                   // command I dont get a fucking stroke
  {
    printf_("H 0 R N E T\n");
    list_PCI_devices();
    // printf("Current uptime (MS): %i\n",PIT::UptmeInMillis);

  } else {
    printf_("No such command as \"%s\" sorry :P\n", args[0]);
  }
}

void DeshUpdate() {
  memset(typedstring, 0, 255);
  idx = 0;
  getstr(typedstring, 255);
  parseCommand(typedstring);
  printf_(">");
}