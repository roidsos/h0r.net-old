#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__
#include <stdint.h>
#include <stdbool.h>
#include <utils/keys.h>

#define KEYBOARD_RELEASE 0x7f

#define KEYBOARD_BUFFER_SIZE 255

#define KEY_IS_PRESS(_s) (!((_s) & KEYBOARD_RELEASE))
#define KEY_IS_RELEASE(_s) (!!((_s) & KEYBOARD_RELEASE))
#define KEY_SCANCODE(_s) ((_s) & 0x7F)
#define KEY_MOD(_s, _m) (!!((_s) & (_m)))
#define KEY_CHAR(_s) KEY_SCANCODE(_s) < 128 ? keyboard_layout_us[KEY_MOD(_s, KEY_MOD_SHIFT) ? 1 : 0][KEY_SCANCODE(_s)] : 0;

struct Keyboard{
    uint16_t mods;
    bool keys[128];
    bool chars[128];

    bool shift;
    bool capslock;
    bool backspace;
};

extern uint8_t keyboard_layout_us[2][128]; 
extern struct Keyboard keyboard;

#define keyboard_key(_s) (keyboard.keys[(_s)])
#define keyboard_char(_c) (keyboard.chars[(uint_8) (_c)])

// ============================================================================
void initkeyboard();
char turn_into_ASCII(uint16_t scancode);
void getstr(char* buffer,int size);
char getch();
#endif // __KEYBOARD_H__