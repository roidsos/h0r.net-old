#include "keyboard.h"
#include <utils/screen.h>
#include <arch/x86_64/interrupts/interrupts.h>
#include <drivers/io/portio.h>
#include <utils/logging/logger.h>

uint8_t keyboard_layout_us[2][128] = {
    {
        KEY_NULL, KEY_ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
        '-', '=', KEY_BACKSPACE, KEY_TAB, 'q', 'w', 'e', 'r', 't', 'y', 'u',
        'i', 'o', 'p', '[', ']', KEY_ENTER, 0, 'a', 's', 'd', 'f', 'g', 'h', 'j',
        'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm',
        ',', '.', '/', 0, 0, 0, ' ', 0, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5,
        KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, 0, 0, KEY_HOME, KEY_UP,
        KEY_PAGE_UP, '-', KEY_LEFT, '5', KEY_RIGHT, '+', KEY_END, KEY_DOWN,
        KEY_PAGE_DOWN, KEY_INSERT, KEY_DELETE, 0, 0, 0, KEY_F11, KEY_F12
    }, {
        KEY_NULL, KEY_ESC, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
        '_', '+', KEY_BACKSPACE, KEY_TAB, 'Q', 'W',   'E', 'R', 'T', 'Y', 'U',
        'I', 'O', 'P',   '{', '}', KEY_ENTER, 0, 'A', 'S', 'D', 'F', 'G', 'H',
        'J', 'K', 'L', ':', '\"', '~', 0, '|', 'Z', 'X', 'C', 'V', 'B', 'N',
        'M', '<', '>', '?', 0, 0, 0, ' ', 0, KEY_F1, KEY_F2, KEY_F3, KEY_F4,
        KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, 0, 0, KEY_HOME, KEY_UP,
        KEY_PAGE_UP, '-', KEY_LEFT, '5', KEY_RIGHT, '+', KEY_END, KEY_DOWN,
        KEY_PAGE_DOWN, KEY_INSERT, KEY_DELETE, 0, 0, 0, KEY_F11, KEY_F12
    }
};

char visible_ascii[102] = {
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '=', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '[', ']', 'a', 's', 'd', 'f', 'g', 'h',
    'j', 'k', 'l', ';', '\'', '`', '\\', 'z', 'x', 'c',
    'v', 'b', 'n', 'm', ',', '.', '/', ' ', '-', '5',
    '+', '!', '@', '#', '$', '%', '^', '&', '*', '(',
    ')', '_', '+', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U',
    'I', 'O', 'P', '{', '}', 'A', 'S', 'D', 'F', 'G',
    'H', 'J', 'K', 'L', ':', '\"', '~', '|', 'Z', 'X',
    'C', 'V', 'B', 'N', 'M', '<', '>', '?', ' ', '-',
    '5', '+',
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

void kb_handler(Registers* regs){
    char scancode = inb8(0x60);
    if(scancode == 0){
        log_error("GOD DAMNIT KEYBOARD NO WORKIE"); 
        EOI(1);
        return;
    }
    log_info("key #%i pressed",scancode);
    EOI(1);
}

void initkeyboard()
{
    keyboard.shift = false;
    keyboard.capslock = false;
    keyboard.backspace = false;

    register_ISR(1,kb_handler);
}

char turn_into_ASCII(uint16_t scancode)
{
     
    if (KEY_SCANCODE(scancode) == KEY_LSHIFT || KEY_SCANCODE(scancode) == KEY_RSHIFT) {
                keyboard.shift = !keyboard.shift;
    }
    if (KEY_IS_PRESS(scancode)) { 
            if (keyboard.shift) { 
               return keyboard_layout_us[1][scancode];
            } else {
               return keyboard_layout_us[0][scancode];
          }
          
    }
    /* } */
    return 0; 
}

void getstr(char* buffer,int size)
{
    int idx = 0;
    while (idx < size) {
        char ch = getch();
        if (ch == 0) {
           // TODO: timer here 100ms
        } else if (is_visible(ch)) {
            buffer[idx++] = ch;
            printchar(ch);
        } else if (ch == KEY_BACKSPACE) {
            if (idx > 0){
                buffer[--idx] = 0;
                Backspace(); 
            }
        } else if (ch == KEY_ENTER) {
            break;
        }
    }
    buffer[idx] = 0;
}

char getch()
{
    return  0; 
}
