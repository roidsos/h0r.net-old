#include <libk/stdtypes.h>

int atoi(char *s) {
    int i = 0;
    int n = 0;
    while (s[i] >= '0' && s[i] <= '9') {
        n = 10 * n + (s[i++] - '0');
    }
    return n;
}

// Halt and catch fire function.
void hcf(void) {
    __asm__ volatile("cli");
    for (;;) {
        __asm__ volatile("hlt");
    }
}
void hlt(void) {
    while (true) {
        __asm__ volatile("hlt");
    }
}
