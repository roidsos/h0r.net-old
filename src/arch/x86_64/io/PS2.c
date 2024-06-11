#include "PS2.h"
#include "input.h"

#include <arch/x86_64/IOAPIC.h>
#include <arch/x86_64/LAPIC.h>
#include <arch/x86_64/interrupts/interrupts.h>
#include <arch/x86_64/io/portio.h>
#include <libk/macros.h>
#include <libk/string.h>
#include <utils/log.h>
#include <vendor/printf.h>

u32 shiftpoint = 0;
u32 capspoint = 0;

char KB_STATE[2] = "";
u32 KB_CAPS_STATE;
u32 KB_SHIFT_STATE;

char *KB_KEY[] = {
    "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
    "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
    "`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=",
};

char *KB_KEY_SHIFT[] = {"~",
                        "!",
                        "@",
                        "#",
                        "$",
                        "%"
                        "^",
                        "&",
                        "*",
                        "(",
                        ")",
                        "_",
                        "+",
                        "{",
                        "}",
                        ":",
                        "\"",
                        "<",
                        ">",
                        "?",
                        "|"};

u8 KB_SCANCODE[] = {0x1e, 0x30, 0x2e, 0x20, 0x12, 0x21, 0x22, 0x23, 0x17,
                         0x24, 0x25, 0x26, 0x32, 0x32, 0x18, 0x19, 0x10, 0x13,
                         0x1f, 0x14, 0x16, 0x2f, 0x11, 0x2d, 0x15, 0x2c, 0x29,
                         0x2,  0x3,  0x4,  0x5,  0x6,  0x7,  0x8,  0x9,  0xa,
                         0xb,  0xc,  0xd,  0x2a, 0x36, 0xaa, 0x3a};

void handler(UNUSED Registers *regs) {
    u8 scancode = in8(PS2_DATA);
    for (u32 i = 0; i < 42; i++) {
        if (scancode == KB_SCANCODE[i]) {
            if (shiftpoint == 1) {
                if (i > 26) {
                    if (scancode != KB_SCANCODE[41] ||
                        scancode != KB_SCANCODE[39] ||
                        scancode != KB_SCANCODE[40]) {
                        KB_CAPS_STATE = 0;
                        KB_SHIFT_STATE = 1;
                        KB_STATE[0] = 0;
                        strcat(KB_STATE, KB_KEY_SHIFT[i - 26]);
                    }
                } else if (i < 26) {
                    KB_CAPS_STATE = 1;
                    KB_SHIFT_STATE = 0;
                    KB_STATE[0] = 0;
                    strcat(KB_STATE, KB_KEY[i]);
                }
            } else if (scancode != KB_SCANCODE[41] ||
                       scancode != KB_SCANCODE[39] ||
                       scancode != KB_SCANCODE[40]) {
                if (i > 38) {
                    KB_STATE[0] = '\0';
                } else if (capspoint == 1) {
                    if (i < 26) {
                        KB_CAPS_STATE = 1;
                        KB_SHIFT_STATE = 0;
                        KB_STATE[0] = 0;
                        strcat(KB_STATE, KB_KEY[i]);
                    } else {
                        KB_CAPS_STATE = 0;
                        KB_SHIFT_STATE = 0;
                        KB_STATE[0] = 0;
                        strcat(KB_STATE, KB_KEY[i]);
                    }
                } else {
                    KB_CAPS_STATE = 0;
                    KB_SHIFT_STATE = 0;
                    KB_STATE[0] = 0;
                    strcat(KB_STATE, KB_KEY[i]);
                }
            }
        } else if (scancode == KB_SCANCODE[39] || scancode == KB_SCANCODE[40]) {
            shiftpoint = 1;
        } else if (scancode == KB_SCANCODE[41]) {
            shiftpoint = 0;
        } else if (scancode == KB_SCANCODE[42]) {
            if (capspoint == 0) {
                capspoint = 1;
            } else {
                capspoint = 0;
            }
        }
    }
    input_init();
    KB_STATE[0] = 0;
    lapic_eoi();
}

void ps2_init() {
    register_ISR(33, handler);
    ioapic_unmask(1);

    if (in8(PS2_COMMAND) & 0x1) // initialize the ps2 controller
        in8(PS2_DATA);
    o8(PS2_COMMAND, 0x60);
    o8(PS2_DATA, 0b01100101);

    o8(PS2_COMMAND, 0xAE);
    o8(PS2_DATA, 0xf4);
}
