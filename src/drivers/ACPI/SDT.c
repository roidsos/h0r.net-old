#include "SDT.h"

_bool do_checksum(sdt_header *table_header) {
    unsigned char sum = 0;

    for (u32 i = 0; i < table_header->length; i++) {
        sum += ((char *)table_header)[i];
    }

    return sum == 0;
}
