#ifndef __TAR_H__
#define __TAR_H__

#ifndef HEAP_ACCESSABLE
#error "The tar parser requires the heap to work, sowwy :P"
#endif

#include <libk/stdtypes.h>

struct tar_header
{
    uint8_t filename[100];
    uint8_t mode[8];
    uint8_t UID[8];
    uint8_t GID[8];
    uint8_t size[12];
    uint8_t mtime[12];
    uint8_t chksum[8];
    uint8_t typeflag;
};

struct tar_contents
{
    uint8_t hdr_num;
    struct tar_header** headers;
};
uint32_t parse_size(uint8_t *in);
struct tar_header* find_file(struct tar_contents* findin,char* name);
struct tar_contents parse_tar(struct tar_header* header,uint64_t file_size);


#endif // __TAR_H__