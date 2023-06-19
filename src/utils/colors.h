#include "utils/types.h"

enum Color {
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
};

// I am porting my rust code to cpp
class ColorCode {
    public:
        ColorCode(Color foreground, Color background) {
            code = (static_cast<uint_8>(background) << 4) | static_cast<uint_8>(foreground);
        }
    private:
        uint_8 code;
};
