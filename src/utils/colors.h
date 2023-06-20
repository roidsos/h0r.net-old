#include "utils/types.h"

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

    uint_8 ColorCode(Color foreground, Color background) {
       return ((uint_8)(background) << 4) | (uint_8)(foreground);
    }

