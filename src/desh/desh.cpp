#include <desh/desh.h>
#include <io/io.h>
#include <lib/printf.h>
#include <drivers/keyboard.h>
#include <drivers/ACPI.h>
#include <drivers/memory/memory.h>
#include <util/string.h>
#include <drivers/pci.h>
#include <drivers/PIT.h>
// whattttt
char typedstring[255];
char idx = 0;
void DeshInit()
{
    printf(">");
}

void parseCommand(char *command)
{
    char **args = split(command, ' ');
    printf("\n");
    if (strcmp(args[0], "version"))
    {
        printf("DEfault SHell v0.00000000000000001 alpha\n");
    }
    else if (strcmp(args[0], "reboot"))
    {
        reboot();
    }
    else if (strcmp(args[0], "echo"))
    {
        for (size_t i = 1; args[i] != 0; i++)
        {
            printf("%s ", args[i]);
        }   
        printf("\n");
    }
    else if(strcmp(args[0],"k"))// shortened it to k so if I try to type the command I dont get a fucking stroke
    {
        printf("H 0 R N E T\n");
        PCI::list_devices();
        printf("Current uptime (MS): %i\n",PIT::UptmeInMillis);

    }
    else
    {
        printf("\n");
    }
}

void DeshUpdate()
{
    memset(typedstring, 0, 255);
    idx = 0;
    getstr(typedstring, 255);
    parseCommand(typedstring);
    printf(">");
}
