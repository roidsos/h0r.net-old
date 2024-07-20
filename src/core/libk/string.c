#include "string.h"
#include <core/libk/stddef.h>
#include <core/libk/stdint.h>

void *memchr(const void *s, int c, usize n) {
    const unsigned char uc = c;
    const unsigned char *su;

    for (su = s; n > 0; ++su, --n) {
        if (*su == uc) {
            return (void *)su;
        }
    }

    return NULL;
}

int memcmp(const void *s1, const void *s2, usize n) {
    const unsigned char *su1;
    const unsigned char *su2;

    for (su1 = s1, su2 = s2; n > 0; ++su1, ++su2, --n) {
        if (*su1 != *su2) {
            return (*su1 < *su2) ? -1 : 1;
        }
    }

    return 0;
}

void *memcpy(void *s1, const void *s2, usize n) {
    // this function assumes the areas do not overlap
    char *su1;
    const char *su2;

    for (su1 = s1, su2 = s2; n > 0; ++su1, ++su2, --n) {
        *su1 = *su2;
    }

    return s1;
}

void *memmove(void *s1, const void *s2, usize n) {
    char *sc1;
    const char *sc2;

    sc1 = s1;
    sc2 = s2;
    if (sc2 < sc1 && sc1 < sc2 + n) {
        for (sc1 += n, sc2 += n; n > 0; --n) {
            *--sc1 = *--sc2;
        }
    } else {
        for (; n > 0; --n) {
            *sc1++ = *sc2++;
        }
    }

    return s1;
}

void *memset(void *s, int c, usize n) {
    const unsigned char uc = c;
    unsigned char *su;

    for (su = s; n > 0; ++su, --n) {
        *su = uc;
    }

    return s;
}

char *strcat(char *s1, const char *s2) {
    char *s;

    for (s = s1; *s != '\0'; ++s)
        ;
    for (; (*s = *s2) != '\0'; ++s, ++s2)
        ;

    return s1;
}

int strcmp(const char *s1, const char *s2) {
    for (; *s1 == *s2; ++s1, ++s2) {
        if (*s1 == '\0') {
            return 0;
        }
    }

    return (*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : 1;
}

char *strcpy(char *s1, const char *s2) {
    char *s = s1;
    for (s = s1; (*s++ = *s2++) != '\0';)
        ;
    return s1;
}

usize strcspn(const char *s1, const char *s2) {
    const char *sc1, *sc2;

    for (sc1 = s1; *sc1 != '\0'; ++sc1) {
        for (sc2 = s2; *sc2 != '\0'; ++sc2) {
            if (*sc1 == *sc2) {
                return sc1 - s1;
            }
        }
    }

    return sc1 - s1;
}

usize strlen(const char *s) {
    const char *sc;

    for (sc = s; *sc != '\0'; ++sc)
        ;
    return sc - s;
}

char *strncat(char *s1, const char *s2, usize n) {
    char *s;

    for (s = s1; *s != '\0'; ++s)
        ;
    for (; n > 0 && *s2 != '\0'; --n) {
        *s++ = *s2++;
    }

    *s = '\0';
    return s1;
}

int strncmp(const char *s1, const char *s2, usize n) {
    for (; n > 0; ++s1, ++s2, --n) {
        if (*s1 != *s2) {
            return (*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : 1;
        } else if (*s1 == '\0') {
            return 0;
        }
    }

    return 0;
}

char *strncpy(char *s1, const char *s2, usize n) {
    char *s;

    for (s = s1; n > 0 && *s2 != '\0'; --n) {
        *s++ = *s2++;
    }

    for (; n > 0; --n) {
        *s++ = '\0';
    }

    return s1;
}

usize strspn(const char *s1, const char *s2) {
    const char *sc1, *sc2;

    for (sc1 = s1; *sc1 != '\0'; ++sc1) {
        for (sc2 = s2;; ++sc2) {
            if (*sc2 == '\0') {
                return sc1 - s1;
            } else if (*sc1 == *sc2) {
                break;
            }
        }
    }

    return sc1 - s1;
}

char *strtok(char *s1, const char *s2) {
    char *sbegin, *send;
    static char *ssave = "";

    sbegin = s1 ? s1 : ssave;
    sbegin += strspn(sbegin, s2);
    if (*sbegin == '\0') {
        ssave = "";
        return NULL;
    }

    send = sbegin + strcspn(sbegin, s2);
    if (*send != '\0') {
        *send++ = '\0';
    }

    ssave = send;
    return sbegin;
}
char *strchr(const char *s, int c) {
    while (*s != '\0') {
        if (*s == (char)c) {
            return (char *)s;
        }
        s++;
    }
    return NULL;
}
