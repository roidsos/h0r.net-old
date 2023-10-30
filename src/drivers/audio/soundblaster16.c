#include "soundblaster16.h"
#include <drivers/Memory/Heap.h>
#include <drivers/Memory/scubadeeznutz.h>
#include <logging/logger.h>
#include <stdbool.h>
#include <vendor/libkrnl-essentials/arch/x86/io/portio.h>
#include <vendor/libkrnl-essentials/types/string.h>

bool sb_working = 0;
void *buf;

void testsb() {
    sb_init();
    sb_play(100);
}

int sb_reset() {
    outb8(0x226, 1);
    outb8(0x226, 0);
    return inb8(0x22A);
}

bool sb_init() {
    buf = calloc(0x0FFF);
    log_info("Allocated %p", VIRT_TO_PHYS(buf));
    // Check if SB_RESET was successful
    if (sb_reset() != 0xAA) {
        sb_working = false;
        log_error("Failed to reset soundblaster");
        return false;
    }

    // Enable speaker
    outb8(0x22C, 0xD1);

    sb_working = true;
    return true;
}

void sb_play(int freq) {
    freq = freq + freq; // Will this shut up the compiler
    if (!sb_working) {
        return;
    }

    // Try to enable
    // Lets pray it works idk
    outb8(0x0A, 5);
    outb8(0X0c, 1);
    outb8(0x0B, 0x49);
    outb8(0x83, ((uint64_t)VIRT_TO_PHYS(buf) >> 16) &
                    0xff); // PAGE TRANSFER (EXAMPLE POSITION IN MEMORY
                           // 0x[01]0F04) - SET THIS VALUE FOR YOU
    outb8(0x02, ((uint64_t)VIRT_TO_PHYS(buf) >> 8) &
                    0xff); // POSITON HIGH BIT (EXAMPLE POSITION IN MEMORY
                           // 0x01[0F]04) - SET THIS VALUE FOR YOU
    outb8(0x02, (uint64_t)VIRT_TO_PHYS(buf) &
                    0xff); // POSITION LOW BIT (EXAMPLE POSITION IN MEMORY
                           // 0x010F[04]) - SET THIS VALUE FOR YOU
    outb8(0x03, 0xFF);
    outb8(0x03, 0x0F);
    outb8(0x0A, 1);
    // THis should DMA channel 1
    // Now lets program it ig
    outb8(0x22C, 0x40);
    outb8(0x22C, 165);
    outb8(0x22C, 0xC0);
    outb8(0x22C, 0x00);
    outb8(0x22C, 0xFE);
    outb8(0x22C, 0x0F);
}