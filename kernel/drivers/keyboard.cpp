#include <drivers/keyboard.h>

uint_8 inportb(uint_16 port) {
    uint_8 r;
    asm("inb %1, %0" : "=a" (r) : "dN" (port));
    return r;
}

uint_8 keyboard_layout_us[2][128] = {
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

struct Keyboard keyboard;

extern "C" void keyint() {
    uint_16 scancode = (uint_16)inportb(0x60);
    if (KEY_SCANCODE(scancode) == KEY_LALT || KEY_SCANCODE(scancode) == KEY_RALT) {

    } else if (KEY_SCANCODE(scancode) == KEY_LCTRL || KEY_SCANCODE(scancode) == KEY_RCTRL) {

    } else if (KEY_SCANCODE(scancode) == KEY_LSHIFT || KEY_SCANCODE(scancode) == KEY_RSHIFT) {
        keyboard.shift = !keyboard.shift;
    } else if (KEY_SCANCODE(scancode) == KEY_CAPS_LOCK) {
        keyboard.capslock = !keyboard.capslock; // Doesnt work, Acts like shift key
    } else if (KEY_SCANCODE(scancode) == KEY_NUM_LOCK) {

    } else if (KEY_SCANCODE(scancode) == KEY_SCROLL_LOCK) {

    } else if (KEY_SCANCODE(scancode) == KEY_BACKSPACE) {
        if (KEY_IS_PRESS(scancode))
            backspace();
    } else if (KEY_SCANCODE(scancode) == KEY_ENTER) {
        if (KEY_IS_PRESS(scancode))
            print("\n");
    } else {
        if (KEY_IS_PRESS(scancode)) {
            if (keyboard.shift) {
                printchar(keyboard_layout_us[1][scancode]);
            } else {
                printchar(keyboard_layout_us[0][scancode]);
            }
        }
        // print(hex2str(scancode));
    }

    keyboard.keys[(uint_8) (scancode & 0x7F)] = KEY_IS_PRESS(scancode);
    keyboard.chars[KEY_CHAR(scancode)] = KEY_IS_PRESS(scancode);
    outb8(0x20,0x20);
    outb8(0xa0,0x20);

};
