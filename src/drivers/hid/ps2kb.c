#include "ps2kb.h"
#include <arch/x86/interrupts/interrupts.h>
#include <arch/x86/io/portio.h>
#include <core/Memory/Heap.h>
#include <core/abstraction/timer.h>
#include <core/kernel.h>
#include <core/logging/logger.h>
#include <flanterm.h>
#include <vendor/printf.h>
char *keybuffer;
int bufindex = 0;
int getindex = 0;
bool bufstuck = false;

#include "core/event-system/event.h"
#include "key-layout.h"

char visible_ascii[102] = {
    '1', '2', '3', '4', '5', '6', '7', '8',  '9',  '0', '-',  '=', 'q',
    'w', 'e', 'r', 't', 'y', 'u', 'i', 'o',  'p',  '[', ']',  'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',  '\'', '`', '\\', 'z', 'x',
    'c', 'v', 'b', 'n', 'm', ',', '.', '/',  ' ',  '-', '5',  '+', '!',
    '@', '#', '$', '%', '^', '&', '*', '(',  ')',  '_', '+',  'Q', 'W',
    'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',  '{',  '}', 'A',  'S', 'D',
    'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~',  '|', 'Z',  'X', 'C',
    'V', 'B', 'N', 'M', '<', '>', '?', ' ',  '-',  '5', '+',
};
struct Keyboard keyboard;

bool is_visible(char keyascii) {
    for (int i = 0; i < 102; i++) {
        if (keyascii == visible_ascii[i]) {
            return true;
        }
    }
    return false;
}

void kb_handler(__attribute__((unused)) Registers *regs) {

    uint8_t scancode = inb8(PS2_DATA);
    if (scancode == 0) {
        log_error("GOD DAMNIT KEYBOARD NO WORKIE");
        EOI(1);
        return;
    }
    if ((bufindex != getindex) || !bufstuck) {
        keybuffer[bufindex] = scancode;
        bufindex = (bufindex + 1) % 128;
        if (bufindex == getindex) {
            bufstuck = true;
        }
    }
    keyboard.keys[(uint8_t)(scancode & 0x7F)] = KEY_IS_PRESS(scancode);
    keyboard.chars[KEY_CHAR(scancode)] = KEY_IS_PRESS(scancode);
    // call_event(1, &keybuffer[bufindex]);
    EOI(1);
}

void kb_init() {
    keybuffer = (char *)calloc(128);
    keyboard.shift = false;
    keyboard.capslock = false;
    keyboard.backspace = false;

    register_ISR(PIC_REMAP_OFFSET + 1, kb_handler);

    if (inb8(PS2_COMMAND) & 0x1) // initialize the ps2 controller
        inb8(PS2_DATA);
    outb8(PS2_COMMAND, 0x60);
    outb8(PS2_DATA, 0b01100111);

    outb8(PS2_COMMAND, 0xAE);
    outb8(PS2_DATA, 0xf4);
    // create_event(1);
}

char turn_into_ASCII(uint16_t scancode) {
    if (KEY_SCANCODE(scancode) == KEY_LSHIFT ||
        KEY_SCANCODE(scancode) == KEY_RSHIFT) {
        keyboard.shift = !keyboard.shift;
    } else if (KEY_SCANCODE(scancode) == KEY_BACKSPACE) {
        return -1;
    }
    if (KEY_IS_PRESS(scancode)) {
        if (keyboard.shift) {
            return keyboard_layout_us[1][scancode];
        } else {
            return keyboard_layout_us[0][scancode];
        }
    }
    return 0;
}

void getstr(char *buffer, int size) {
    int idx = 0;
    bool repeat = false;
    int repeat_count = 0;
    int repeat_delay = 70;

    do {
        char _char = getch();
        if (_char) {
            if (_char == -1 && idx != 0) {
                buffer[idx--] = ' ';
                flanterm_write(data.ft_ctx, "\b \b", 3);
            } else {
                if (repeat) {
                    if (++repeat_count >= 10) {
                        repeat_count = 0;
                        if (is_visible(_char)) {
                            printf("%c", _char);
                            buffer[idx] = _char;
                            idx++;
                        }
                    }
                } else {
                    if (is_visible(_char)) {
                        printf("%c", _char);
                        buffer[idx] = _char;
                        idx++;
                    }
                }
                repeat = true;
            }
        } else {
            repeat = false;
            repeat_count = 0;
        }
        if (idx >= size)
            return;
        timer_sleep(repeat ? repeat_delay / 10 : repeat_delay);
    } while (!keyboard_key(KEY_ENTER));
    buffer[idx] = 0;
    timer_sleep(100);
}

char getch() {
    if ((bufindex != getindex) || bufstuck) {
        if (bufstuck) {
            bufstuck = false;
        }
        char keyout = keybuffer[getindex];
        keybuffer[getindex] = 0;
        getindex = (getindex + 1) % 128;
        return turn_into_ASCII(keyout);
    }
    return 0;
}
