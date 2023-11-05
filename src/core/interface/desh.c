#include "desh.h"
#include <arch/x86/power.h>
#include <core/Memory/Heap.h>
#include <core/Memory/PFA.h>
#include <core/VFS/vfs.h>
#include <core/abstraction/hwrand.h>
#include <core/abstraction/timer.h>
#include <core/kernel.h>
#include <drivers/audio/soundblaster16.h>
#include <drivers/hid/ps2kb.h>
#include <drivers/io/pci.h>
#include <klibc/memory.h>
#include <types/string.h>
#include <vendor/printf.h>

char typedstring[255];
char currentpath[255];

void DeshInit() {
    memset(typedstring, 0, 255);
    memset(currentpath, 0, 255);
    currentpath[0] = '/';
    printf("%s >", currentpath);
}

void parseCommand(char *command) {
    printf("\n");
    char *token = strtok(command, " ");
    char **args = malloc(sizeof(char) * 255);
    int argCount = 0;

    while (token != NULL) {
        args[argCount] = token;
        argCount++;
        token = strtok(NULL, " ");
    }
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
        printf("Current uptime %u secs %u ms\n", timer_get_uptime_secs(),
               timer_get_uptime_milis());
        printf("Total system memory: %llu bytes\n", get_total_RAM());
        printf("Free system memory: %llu bytes\n", get_free_RAM());
        printf("Used system memory: %llu bytes\n", get_used_RAM());
    } else if (strcmp(args[0], "") == 0) {

    } else if (strcmp(args[0], "ls") == 0) {
        if (!vfs_is_dir(currentpath)) {
            printf("you are not in a directory???\n");
        } else {
            struct dir_report nodes = vfs_iterate_dir(currentpath);
            if (nodes.num_entries == 0) {
                printf("empty");
            }
            for (size_t i = 0; i < nodes.num_entries; i++) {
                printf("%s ", nodes.entries[i].name);
            }
            printf("\n");
        }

    } else if (strcmp(args[0], "cd") == 0) {
        char path[255];
        if (args[1][0] == '/') {
            memcpy(path, args[1], 255);
        } else {
            if (currentpath[strlen(currentpath) - 1] == '/') {
                snprintf(path, 255, "%s%s", currentpath, args[1]);
            } else {
                snprintf(path, 255, "%s/%s", currentpath, args[1]);
            }
        }
        if (args[1] == 0 || !vfs_is_dir(path)) {
            printf("you need to provide a DIRECTORY to change to :P\n");
        } else {
            strcpy(currentpath, path);
        }

    } else if (strcmp(args[0], "cat") == 0) {
        char path[255];
        if (args[1][0] == '/') {
            memcpy(path, args[1], 255);
        } else {
            if (currentpath[strlen(currentpath) - 1] == '/') {
                snprintf(path, 255, "%s%s", currentpath, args[1]);
            } else {
                snprintf(path, 255, "%s/%s", currentpath, args[1]);
            }
        }
        if (vfs_inspect(path) == NULL) {
            printf("desh: no such file or directory \"%s\"\n", path);
        } else if (vfs_is_dir(path)) {
            printf("\"%s\" is a directory, moron :P\n", path);
        } else {
            printf("%s\n", (char *)vfs_read(path, 0, 2000).data);
        }
    } else if (strcmp(args[0], "testsb") == 0) {
        printf("Testing soundblaster16\n");
        testsb();
    } else if (strcmp(args[0], "dbginfo") == 0) {
        printf("  UEFI mode: %d\n", data.is_uefi_mode);
        if (data.is_uefi_mode)
            printf("  EFI System Table Address: 0x%p",
                   data.efi_system_table_address);

        printf("  Memmap entry count: %lu\n", data.memmap_resp->entry_count);
        for (size_t i = 0; i < data.memmap_resp->entry_count; i++) {
            printf(
                "   -Memmap entry #%lu: Base: 0x%lx, Length: 0x%lx, Type:%s\n",
                i, data.memmap_resp->entries[i]->base,
                data.memmap_resp->entries[i]->length,
                memmap_type_names[data.memmap_resp->entries[i]->type]);
        }

        printf("  CPU count: %u\n", data.smp_resp->cpu_count);
        for (size_t i = 0; i < data.smp_resp->cpu_count; i++) {
            struct limine_smp_info *cpu = data.smp_resp->cpus[i];
            printf("   -Core #%u Extra Argument: %u\n", i, cpu->extra_argument);
            printf("   -Core #%u Goto Address: %u\n", i, cpu->goto_address);
            printf("   -Core #%u Lapic ID: %u\n", i, cpu->lapic_id);
            printf("   -Core #%u Processor ID: %u\n\n", i, cpu->processor_id);
        }
        printf("  CPU Vendor ID: %s\n", data.cpu_info.vendor);
        printf("  CPU Family: %d\n", data.cpu_info.family);
        printf("  CPU Model: %d\n", data.cpu_info.model);
        printf("  CPU Stepping: %d\n", data.cpu_info.stepping);
        printf("  CPU X-family: %d\n", data.cpu_info.ext_family);
        printf("  CPU X-model: %d\n", data.cpu_info.ext_model);
        printf("  CPU Capabilities(EDX): 0x%.8X\n", data.cpu_info.features[0]);
        printf("  CPU Capabilities(ECX): 0x%.8X\n", data.cpu_info.features[1]);
    } else if (strcmp(args[0], "rand") == 0) {
        printf("Number: %u\n", rand(HWRAND_SRC_IDC));
    } else {
        printf("No such command as \"%s\" sorry :P\n", args[0]);
    }
    free(args);
}

void DeshUpdate() {
    getstr(typedstring, 255);
    parseCommand(typedstring);

    printf("%s >", currentpath);
}