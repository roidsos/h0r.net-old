#include "log.h"
#include <drivers/output/cereal.h>
#include <vendor/printf.h>
char *levels[] = {"\x1b[36mTRAC\033[0m", "\033[32mDBUG\033[0m",
                  "\x1b[94mINFO\033[0m", "\033[32mNICE\033[0m",
                  "\033[33mWARN\033[0m", "\033[31mEROR\033[0m"};

int level_ports[] = {TRAC_PORT, DBUG_PORT, INFO_PORT,
                     NICE_PORT, WARN_PORT, EROR_PORT};

void log(u8 level, const char *file, u32 line, const char *func,
         char *msg, ...) {
    va_list va;
    va_start(va, msg);

    char msgbuf[1024];
    vsnprintf(msgbuf, 1024, msg, va);

    char fulbuf[4096]; // this isn't PAGE_SIZE...
    snprintf(fulbuf, 4096, "\x1b[37m[%s\x1b[37m, %s:%u(%s)]\033[0m %s",
             levels[level], file, line, func, msgbuf);

    for (usize i = 0; fulbuf[i]; i++) {
        cereal_write(fulbuf[i], level_ports[level]);
    }

    va_end(va);
}
