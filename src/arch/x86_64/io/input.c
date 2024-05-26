#include "PS2.h"
#include "vendor/printf.h"
#include "input.h"
#include <string.h>

char *CAPS_LETTERS[] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", 
                         "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T",
                         "U", "V", "W", "X", "Y", "Z" }; 

char *CHARS[] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", 
                  "k", "l", "m", "n", "o", "p", "q", "r", "s", "t",
                  "u", "v", "w", "x", "y", "z", "`", "1", "2", "3", 
                  "4", "5", "6", "7", "8", "9", "0", "-", "=", "-",
                  "=" }; 

char *SHIFT_CHARS[] = { "~", "!", "@", "#", "$", "%", "^", "&", "*",
                        "(", ")", "_", "+", "{", "}", ":", "\"","<",
                        ">", "?", "|" };

char *IDS[] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", 
                "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T",
                "U", "V", "W", "X", "Y", "Z", "`", "1", "2", "3", 
                "4", "5", "6", "7", "8", "9", "0", "-", "=", "~",
                "!", "@", "#", "$", "%", "^", "&", "*", "(", ")",
                "_", "+", "{", "}", ":", "\"","<", ">", "?", "|" }; 

char *SPECIAL_CHARS[] = { "^[[A" };

void input_init() { 
    for (int i = 0; i < 59; i++) {
        if (!strcmp(KB_STATE, IDS[i])) {
            if (KB_CAPS_STATE == 1) {
                printf("%s", CAPS_LETTERS[i]);
            } else if (KB_SHIFT_STATE == 1) {
                printf("%s", SHIFT_CHARS[i-39]);
            } else {
                printf("%s", CHARS[i]);
            }
        }
    }
}