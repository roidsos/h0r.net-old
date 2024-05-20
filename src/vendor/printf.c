///////////////////////////////////////////////////////////////////////////////
// \author (c) Marco Paland (info@paland.com)
//             2014-2019, PALANDesign Hannover, Germany
//
// \license The MIT License (MIT)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// \brief Tiny printf and (v)snprintf implementation, optimized for
// speed on
//        embedded systems with a very limited resources. These routines are
//        thread safe and reentrant! Use this instead of the bloated
//        standard/newlib printf cause these use malloc for printf (and may not
//        be thread safe).
//
// Note: Edited
///////////////////////////////////////////////////////////////////////////////
#include "printf.h"
#include <arch/x86_64/drivers/output/cereal.h>
#include <config.h>
#include <core/kernel.h>
#include <core/sys/resman/tty.h>
#include <core/sys/sched/sched.h>
#include <libk/macros.h>
#include <libk/stdint.h>
#include <uterus.h>

// 'ntoa' conversion buffer size, this must be big enough to hold one converted
// numeric number including padded zeros (dynamically created on stack)
// default: 32 byte
#ifndef PRINTF_NTOA_BUFFER_SIZE
#define PRINTF_NTOA_BUFFER_SIZE 32U
#endif

// support for the long long types (%llu or %p)
// default: activated
#ifndef PRINTF_DISABLE_SUPPORT_LONG_LONG
#define PRINTF_SUPPORT_LONG_LONG
#endif

///////////////////////////////////////////////////////////////////////////////

// internal flag definitions
#define FLAGS_ZEROPAD (1U << 0U)
#define FLAGS_LEFT (1U << 1U)
#define FLAGS_PLUS (1U << 2U)
#define FLAGS_SPACE (1U << 3U)
#define FLAGS_HASH (1U << 4U)
#define FLAGS_UPPERCASE (1U << 5U)
#define FLAGS_CHAR (1U << 6U)
#define FLAGS_SHORT (1U << 7U)
#define FLAGS_LONG (1U << 8U)
#define FLAGS_LONG_LONG (1U << 9U)
#define FLAGS_PRECISION (1U << 10U)

// output function type
typedef void (*out_fct_type)(char character, void *buffer, usize idx,
                             usize maxlen);

// wrapper (used as buffer) for output function type
typedef struct {
    void (*fct)(char character, void *arg);
    void *arg;
} out_fct_wrap_type;

// internal buffer output
static inline void _out_buffer(char character, void *buffer, usize idx,
                               usize maxlen) {
    if (idx < maxlen) {
        ((char *)buffer)[idx] = character;
    }
}

static inline void _out_dbg(char character, UNUSED void *buffer,
                            UNUSED usize idx, UNUSED usize maxlen) {
    if (character == 0)
        return;
    cereal_write(character,
                 COM1); // port E9 trick, only works on bochs and qemu, doesnt
                        // break real hardware( unless the hardware is garbage)
}

// internal null output
static inline void _out_null(char character, void *buffer, usize idx,
                             usize maxlen) {
    (void)character;
    (void)buffer;
    (void)idx;
    (void)maxlen;
}

typedef struct {
    u16 tty_id;
    u32 victim_pid;
} lock_victim_t;

lock_victim_t locks[MAX_LOCKED_PROCS];

void init_printf_locks() {
    for (int i = 0; i < MAX_LOCKED_PROCS; i++) {
        locks[i].tty_id = 0;
        locks[i].victim_pid = 0;
    }
}

// internal _putchar wrapper
static inline void _out_char(char character, UNUSED void *buffer,
                             UNUSED usize idx, UNUSED usize maxlen) {
    if (sched_running && tty_initialized) {
        // TODO: move this into its own place
        u16 curr_tty = sched_get_curr_process()->tty_id;
        if (!tty_lock(curr_tty)) {
            for (int i = 0; i < MAX_LOCKED_PROCS; i++) {
                if (locks[i].tty_id == 0) {
                    locks[i].tty_id = curr_tty;
                    locks[i].victim_pid = sched_current_pid;
                    sched_block(sched_current_pid);
                }
            }
            while (tty_get_lock(curr_tty))
                ;
        }
        tty_write(curr_tty, &character, 1);
        tty_unlock(curr_tty);
        for (int i = 0; i < MAX_LOCKED_PROCS; i++) {
            if (locks[i].tty_id == curr_tty) {
                sched_unblock(locks[i].victim_pid);
                locks[i].tty_id = 0;
                locks[i].victim_pid = 0;
            }
        }
    } else {
        uterus_write(data.ut_ctx, &character, 1);
    }
}

// internal secure strlen
// \return The length of the string (excluding the terminating 0) limited by
// 'maxsize'
static inline unsigned int _strnlen_s(const char *str, usize maxsize) {
    const char *s;
    for (s = str; *s && maxsize--; ++s)
        ;
    return (unsigned int)(s - str);
}

// internal test if char is a digit (0-9)
// \return true if char is a digit
static inline _bool _is_digit(char ch) { return (ch >= '0') && (ch <= '9'); }

// internal ASCII string to unsigned int conversion
static unsigned int _atoi(const char **str) {
    unsigned int i = 0U;
    while (_is_digit(**str)) {
        i = i * 10U + (unsigned int)(*((*str)++) - '0');
    }
    return i;
}

// output the specified string in reverse, taking care of any zero-padding
static usize _out_rev(out_fct_type out, char *buffer, usize idx, usize maxlen,
                      const char *buf, usize len, unsigned int width,
                      unsigned int flags) {
    const usize start_idx = idx;

    // pad spaces up to given width
    if (!(flags & FLAGS_LEFT) && !(flags & FLAGS_ZEROPAD)) {
        for (usize i = len; i < width; i++) {
            out(' ', buffer, idx++, maxlen);
        }
    }

    // reverse string
    while (len) {
        out(buf[--len], buffer, idx++, maxlen);
    }

    // append pad spaces up to given width
    if (flags & FLAGS_LEFT) {
        while (idx - start_idx < width) {
            out(' ', buffer, idx++, maxlen);
        }
    }

    return idx;
}

// internal itoa format
static usize _ntoa_format(out_fct_type out, char *buffer, usize idx,
                          usize maxlen, char *buf, usize len, _bool negative,
                          unsigned int base, unsigned int prec,
                          unsigned int width, unsigned int flags) {
    // pad leading zeros
    if (!(flags & FLAGS_LEFT)) {
        if (width && (flags & FLAGS_ZEROPAD) &&
            (negative || (flags & (FLAGS_PLUS | FLAGS_SPACE)))) {
            width--;
        }
        while ((len < prec) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
            buf[len++] = '0';
        }
        while ((flags & FLAGS_ZEROPAD) && (len < width) &&
               (len < PRINTF_NTOA_BUFFER_SIZE)) {
            buf[len++] = '0';
        }
    }

    // handle hash
    if (flags & FLAGS_HASH) {
        if (!(flags & FLAGS_PRECISION) && len &&
            ((len == prec) || (len == width))) {
            len--;
            if (len && (base == 16U)) {
                len--;
            }
        }
        if ((base == 16U) && !(flags & FLAGS_UPPERCASE) &&
            (len < PRINTF_NTOA_BUFFER_SIZE)) {
            buf[len++] = 'x';
        } else if ((base == 16U) && (flags & FLAGS_UPPERCASE) &&
                   (len < PRINTF_NTOA_BUFFER_SIZE)) {
            buf[len++] = 'X';
        } else if ((base == 2U) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
            buf[len++] = 'b';
        }
        if (len < PRINTF_NTOA_BUFFER_SIZE) {
            buf[len++] = '0';
        }
    }

    if (len < PRINTF_NTOA_BUFFER_SIZE) {
        if (negative) {
            buf[len++] = '-';
        } else if (flags & FLAGS_PLUS) {
            buf[len++] = '+'; // ignore the space if the '+' exists
        } else if (flags & FLAGS_SPACE) {
            buf[len++] = ' ';
        }
    }

    return _out_rev(out, buffer, idx, maxlen, buf, len, width, flags);
}

// internal itoa for 'long' type
static usize _ntoa_long(out_fct_type out, char *buffer, usize idx, usize maxlen,
                        unsigned long value, _bool negative, unsigned long base,
                        unsigned int prec, unsigned int width,
                        unsigned int flags) {
    char buf[PRINTF_NTOA_BUFFER_SIZE];
    usize len = 0U;

    // no hash for 0 values
    if (!value) {
        flags &= ~FLAGS_HASH;
    }

    // write if precision != 0 and value is != 0
    if (!(flags & FLAGS_PRECISION) || value) {
        do {
            const char digit = (char)(value % base);
            buf[len++] =
                digit < 10 ? '0' + digit
                           : (flags & FLAGS_UPPERCASE ? 'A' : 'a') + digit - 10;
            value /= base;
        } while (value && (len < PRINTF_NTOA_BUFFER_SIZE));
    }

    return _ntoa_format(out, buffer, idx, maxlen, buf, len, negative,
                        (unsigned int)base, prec, width, flags);
}

// internal itoa for 'long long' type
#if defined(PRINTF_SUPPORT_LONG_LONG)
static usize _ntoa_long_long(out_fct_type out, char *buffer, usize idx,
                             usize maxlen, unsigned long long value,
                             _bool negative, unsigned long long base,
                             unsigned int prec, unsigned int width,
                             unsigned int flags) {
    char buf[PRINTF_NTOA_BUFFER_SIZE];
    usize len = 0U;

    // no hash for 0 values
    if (!value) {
        flags &= ~FLAGS_HASH;
    }

    // write if precision != 0 and value is != 0
    if (!(flags & FLAGS_PRECISION) || value) {
        do {
            const char digit = (char)(value % base);
            buf[len++] =
                digit < 10 ? '0' + digit
                           : (flags & FLAGS_UPPERCASE ? 'A' : 'a') + digit - 10;
            value /= base;
        } while (value && (len < PRINTF_NTOA_BUFFER_SIZE));
    }

    return _ntoa_format(out, buffer, idx, maxlen, buf, len, negative,
                        (unsigned int)base, prec, width, flags);
}
#endif // PRINTF_SUPPORT_LONG_LONG

// internal vsnprintf
static int _vsnprintf(out_fct_type out, char *buffer, const usize maxlen,
                      const char *format, va_list va) {
    unsigned int flags, width, precision, n;
    usize idx = 0U;

    if (!buffer) {
        // use null output function
        out = _out_null;
    }

    while (*format) {
        // format specifier?  %[flags][width][.precision][length]
        if (*format != '%') {
            // no
            out(*format, buffer, idx++, maxlen);
            format++;
            continue;
        } else {
            // yes, evaluate it
            format++;
        }

        // evaluate flags
        flags = 0U;
        do {
            switch (*format) {
            case '0':
                flags |= FLAGS_ZEROPAD;
                format++;
                n = 1U;
                break;
            case '-':
                flags |= FLAGS_LEFT;
                format++;
                n = 1U;
                break;
            case '+':
                flags |= FLAGS_PLUS;
                format++;
                n = 1U;
                break;
            case ' ':
                flags |= FLAGS_SPACE;
                format++;
                n = 1U;
                break;
            case '#':
                flags |= FLAGS_HASH;
                format++;
                n = 1U;
                break;
            default:
                n = 0U;
                break;
            }
        } while (n);

        // evaluate width field
        width = 0U;
        if (_is_digit(*format)) {
            width = _atoi(&format);
        } else if (*format == '*') {
            const int w = va_arg(va, int);
            if (w < 0) {
                flags |= FLAGS_LEFT; // reverse padding
                width = (unsigned int)-w;
            } else {
                width = (unsigned int)w;
            }
            format++;
        }

        // evaluate precision field
        precision = 0U;
        if (*format == '.') {
            flags |= FLAGS_PRECISION;
            format++;
            if (_is_digit(*format)) {
                precision = _atoi(&format);
            } else if (*format == '*') {
                const int prec = (int)va_arg(va, int);
                precision = prec > 0 ? (unsigned int)prec : 0U;
                format++;
            }
        }

        // evaluate length field
        switch (*format) {
        case 'l':
            flags |= FLAGS_LONG;
            format++;
            if (*format == 'l') {
                flags |= FLAGS_LONG_LONG;
                format++;
            }
            break;
        case 'h':
            flags |= FLAGS_SHORT;
            format++;
            if (*format == 'h') {
                flags |= FLAGS_CHAR;
                format++;
            }
            break;
        case 'j':
            flags |= (sizeof(intmax_t) == sizeof(long) ? FLAGS_LONG
                                                       : FLAGS_LONG_LONG);
            format++;
            break;
        case 'z':
            flags |=
                (sizeof(usize) == sizeof(long) ? FLAGS_LONG : FLAGS_LONG_LONG);
            format++;
            break;
        default:
            break;
        }

        // evaluate specifier
        switch (*format) {
        case 'd':
        case 'i':
        case 'u':
        case 'x':
        case 'X':
        case 'o':
        case 'b': {
            // set the base
            unsigned int base;
            if (*format == 'x' || *format == 'X') {
                base = 16U;
            } else if (*format == 'o') {
                base = 8U;
            } else if (*format == 'b') {
                base = 2U;
            } else {
                base = 10U;
                flags &= ~FLAGS_HASH; // no hash for dec format
            }
            // uppercase
            if (*format == 'X') {
                flags |= FLAGS_UPPERCASE;
            }

            // no plus or space flag for u, x, X, o, b
            if ((*format != 'i') && (*format != 'd')) {
                flags &= ~(FLAGS_PLUS | FLAGS_SPACE);
            }

            // ignore '0' flag when precision is given
            if (flags & FLAGS_PRECISION) {
                flags &= ~FLAGS_ZEROPAD;
            }

            // convert the integer
            if ((*format == 'i') || (*format == 'd')) {
                // signed
                if (flags & FLAGS_LONG_LONG) {
#if defined(PRINTF_SUPPORT_LONG_LONG)
                    const long long value = va_arg(va, long long);
                    idx = _ntoa_long_long(
                        out, buffer, idx, maxlen,
                        (unsigned long long)(value > 0 ? value : 0 - value),
                        value < 0, base, precision, width, flags);
#endif
                } else if (flags & FLAGS_LONG) {
                    const long value = va_arg(va, long);
                    idx = _ntoa_long(
                        out, buffer, idx, maxlen,
                        (unsigned long)(value > 0 ? value : 0 - value),
                        value < 0, base, precision, width, flags);
                } else {
                    const int value =
                        (flags & FLAGS_CHAR)    ? (char)va_arg(va, int)
                        : (flags & FLAGS_SHORT) ? (short int)va_arg(va, int)
                                                : va_arg(va, int);
                    idx = _ntoa_long(
                        out, buffer, idx, maxlen,
                        (unsigned int)(value > 0 ? value : 0 - value),
                        value < 0, base, precision, width, flags);
                }
            } else {
                // unsigned
                if (flags & FLAGS_LONG_LONG) {
#if defined(PRINTF_SUPPORT_LONG_LONG)
                    idx = _ntoa_long_long(out, buffer, idx, maxlen,
                                          va_arg(va, unsigned long long), false,
                                          base, precision, width, flags);
#endif
                } else if (flags & FLAGS_LONG) {
                    idx = _ntoa_long(out, buffer, idx, maxlen,
                                     va_arg(va, unsigned long), false, base,
                                     precision, width, flags);
                } else {
                    const unsigned int value =
                        (flags & FLAGS_CHAR)
                            ? (unsigned char)va_arg(va, unsigned int)
                        : (flags & FLAGS_SHORT)
                            ? (unsigned short int)va_arg(va, unsigned int)
                            : va_arg(va, unsigned int);
                    idx = _ntoa_long(out, buffer, idx, maxlen, value, false,
                                     base, precision, width, flags);
                }
            }
            format++;
            break;
        }
        case 'c': {
            unsigned int l = 1U;
            // pre padding
            if (!(flags & FLAGS_LEFT)) {
                while (l++ < width) {
                    out(' ', buffer, idx++, maxlen);
                }
            }
            // char output
            out((char)va_arg(va, int), buffer, idx++, maxlen);
            // post padding
            if (flags & FLAGS_LEFT) {
                while (l++ < width) {
                    out(' ', buffer, idx++, maxlen);
                }
            }
            format++;
            break;
        }

        case 's': {
            const char *p = va_arg(va, char *);
            unsigned int l = _strnlen_s(p, precision ? precision : (usize)-1);
            // pre padding
            if (flags & FLAGS_PRECISION) {
                l = (l < precision ? l : precision);
            }
            if (!(flags & FLAGS_LEFT)) {
                while (l++ < width) {
                    out(' ', buffer, idx++, maxlen);
                }
            }
            // string output
            while ((*p != 0) && (!(flags & FLAGS_PRECISION) || precision--)) {
                out(*(p++), buffer, idx++, maxlen);
            }
            // post padding
            if (flags & FLAGS_LEFT) {
                while (l++ < width) {
                    out(' ', buffer, idx++, maxlen);
                }
            }
            format++;
            break;
        }

        case 'p': {
            width = sizeof(void *) * 2U;
            flags |= FLAGS_ZEROPAD | FLAGS_UPPERCASE;
#if defined(PRINTF_SUPPORT_LONG_LONG)
            const _bool is_ll = sizeof(uptr) == sizeof(long long);
            if (is_ll) {
                idx = _ntoa_long_long(out, buffer, idx, maxlen,
                                      (uptr)va_arg(va, void *), false, 16U,
                                      precision, width, flags);
            } else {
#endif
                idx = _ntoa_long(out, buffer, idx, maxlen,
                                 (unsigned long)((uptr)va_arg(va, void *)),
                                 false, 16U, precision, width, flags);
#if defined(PRINTF_SUPPORT_LONG_LONG)
            }
#endif
            format++;
            break;
        }

        case '%':
            out('%', buffer, idx++, maxlen);
            format++;
            break;

        default:
            out(*format, buffer, idx++, maxlen);
            format++;
            break;
        }
    }

    // termination
    out((char)0, buffer, idx < maxlen ? idx : maxlen - 1U, maxlen);

    // return written chars without terminating \0
    return (int)idx;
}

///////////////////////////////////////////////////////////////////////////////

int printf(const char *format, ...) {
    va_list va;
    va_start(va, format);
    char buffer[1];
    const int ret = _vsnprintf(_out_char, buffer, (usize)-1, format, va);
    va_end(va);
    return ret;
}

int dprintf(const char *format, ...) {
    va_list va;
    va_start(va, format);
    char buffer[1];
    const int ret = _vsnprintf(_out_dbg, buffer, (usize)-1, format, va);
    va_end(va);
    return ret;
}

int snprintf(char *buffer, usize count, const char *format, ...) {
    va_list va;
    va_start(va, format);
    const int ret = _vsnprintf(_out_buffer, buffer, count, format, va);
    va_end(va);
    return ret;
}

int vprintf(const char *format, va_list va) {
    char buffer[1];
    return _vsnprintf(_out_char, buffer, (usize)-1, format, va);
}

int vsnprintf(char *buffer, usize count, const char *format, va_list va) {
    return _vsnprintf(_out_buffer, buffer, count, format, va);
}
