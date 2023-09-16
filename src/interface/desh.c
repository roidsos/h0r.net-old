#include "desh.h"
#include <arch/x86/PIT.h>
#include <arch/x86/essential.h>
#include <arch/x86/power.h>
#include <drivers/hid/keyboard.h>
#include <drivers/io/pci.h>
#include <flanterm.h>
#include <font/font_renderer.h>
#include <kernel.h>
#include <types/string.h>
#include <vendor/printf.h>
#include <VFS/drive_manager.h>

char typedstring[255];
char currentpath[255];
char idx;
void DeshInit() {
    memset(typedstring, 0, 255);
    memset(currentpath, 0, 255);
    currentpath[0] = '/';
    printf("%s >",currentpath);
}

void parseCommand(char *command) {
    char **args = split(command, ' ');
    printf("\n");
    if (strcmp(args[0], "version")) {
        printf("DEfault SHell v0.00000000000000001 alpha\n");
    } else if (strcmp(args[0], "reboot")) {
        sys_reboot();
    } else if (strcmp(args[0], "clear") || strcmp(args[0], "cls")) {
        flanterm_write(data.ft_ctx, "\e[2J\e[H", 7);
    } else if (strcmp(args[0], "echo")) {
        for (size_t i = 1; args[i] != 0; i++) {
            printf("%s ", args[i]);
        }
        printf("\n");
    } else if (strcmp(args[0], "k")) {
        printf("H 0 R N E T\n");
        list_PCI_devices();
        printf("Current uptime %u secs %u ms\n", pit_get_uptime_secs(),
               pit_get_uptime_milis());
    } else if (strcmp(args[0], "")) {

    } else if (strcmp(args[0], "ls")) {
        struct node* folder_contents = get_dir_contents(0,currentpath);
        for (size_t i = 0;; i++)
        {
            if (!folder_contents[i].flags)
            {
                break;
            }
            printf("%s ",folder_contents->name);
        }
    } else if (strcmp(args[0], "cd")) {
        if (args[1] == 0)
        {
            printf("you need to provide the folder to change to :P\n");
        }else if (args[1][0] == '/')
        {
            memcpy(currentpath,args[1],strlen(args[1]) + 1);
        }else{
            char currpcopy[255];
            snprintf(currpcopy,255,"%s%s/",currentpath,args[1]);
            memcpy(currentpath,currpcopy,255);
        }
        
    } else {

        printf("No such command as \"%s\" sorry :P\n", args[0]);
    }
}

void DeshUpdate() {
    idx = 0;
    getstr(typedstring, 255);
    parseCommand(typedstring);

    printf("%s >",currentpath);
}