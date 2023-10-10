#include "desh.h"
#include <VFS/vfs.h>
#include <arch/x86/PIT.h>
#include <klibc/memory.h>
#include <arch/x86/power.h>
#include <drivers/hid/keyboard.h>
#include <drivers/io/pci.h>
#include <flanterm.h>
#include <font/font_renderer.h>
#include <kernel.h>
#include <types/string.h>
#include <vendor/printf.h>

char typedstring[255];
char currentpath[255];
char idx;

void DeshInit() {
    memset(typedstring, 0, 255);
    memset(currentpath, 0, 255);
    currentpath[0] = '/';
    printf("%s >", currentpath);
}

void parseCommand(char *command) {
    char **args = split(command, ' ');
    printf("\n");
    if (strcmp(args[0], "version") == 0) {
        printf("DEfault SHell v0.00000000000000001 alpha\n");
    } else if (strcmp(args[0], "reboot") == 0) {
        sys_reboot();
    } else if (strcmp(args[0], "clear") == 0 || strcmp(args[0], "cls") == 0) {
        flanterm_write(data.ft_ctx, "\e[2J\e[H", 7);
    } else if (strcmp(args[0], "echo") == 0) {
        for (size_t i = 1; args[i] != 0; i++) {
            printf("%s ", args[i]);
        }
        printf("\n");
    } else if (strcmp(args[0], "k") == 0) {
        printf("H 0 R N E T\n");
        list_PCI_devices();
        printf("Current uptime %u secs %u ms\n", pit_get_uptime_secs(),
               pit_get_uptime_milis());
    } else if (strcmp(args[0], "") == 0) {

    } else if (strcmp(args[0], "ls") == 0) {
        if (!vfs_is_dir(currentpath)) {
            printf("you are not in a directory...\n");
        } else {
            struct dir_report nodes = vfs_iterate_dir(currentpath);
            if (nodes.num_entries == 0){
                printf("empty");
            }
            for (size_t i = 0; i < nodes.num_entries; i++)
            {
                printf("-\"%s\" \n",nodes.entries[i].name);

            }
            printf("\n");
        }

    } else if (strcmp(args[0], "cd") == 0) {
        if (args[1] == 0 || !vfs_is_dir(args[1])) {
            printf("you need to provide a DIRECTORY to change to :P\n");
        } else if (args[1][0] == '/') {
            memcpy(currentpath, args[1], strlen(args[1]) + 1);
        } else {
            char currpcopy[255];
            snprintf(currpcopy, 255, "%s%s/", currentpath, args[1]);
            memcpy(currentpath, currpcopy, 255);
        }
    } else if (strcmp(args[0], "cat") == 0) {
        if (vfs_is_dir(args[1])) {
            printf("you cant cat a directory, moron :P\n");
        } else {
            printf("%s\n", (char*)vfs_read(args[1],0,2000).data);
        }
    } else {
        printf("No such command as \"%s\" sorry :P\n", args[0]);
    }
}

void DeshUpdate() {
    idx = 0;
    getstr(typedstring, 255);
    parseCommand(typedstring);

    printf("%s >", currentpath);
}