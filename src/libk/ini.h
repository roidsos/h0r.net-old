#ifndef __INI_H__
#define __INI_H__
#include <libk/stdtypes.h>

#ifndef HEAP_ACCESSABLE
#error "The ini parser requires the heap to work, sowwy :P"
#endif

struct key_value_pair
{
    char* key;
    char* value;
};
struct parsed_ini{
    struct key_value_pair* data;
    uint8_t count;
};
struct parsed_ini parse_ini(char* string);

#endif // __INI_H__