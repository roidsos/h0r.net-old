#ifndef __COLORS_H__
#define __COLORS_H__

typedef enum {
    Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Magenta = 5,
    Brown = 6,
    LightGray = 7,
    DarkGray = 8,
    LightBlue = 9,
    LightGreen = 10,
    LightCyan = 11,
    LightRed = 12,
    Pink = 13,
    Yellow = 14,
    White = 15
} Color;

char ColorCode(Color foreground, Color background) {
    return ((char)(background) << 4) | (char)(foreground);
}


#endif // __COLORS_H__