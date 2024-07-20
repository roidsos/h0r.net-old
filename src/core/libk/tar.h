#ifndef __TAR_H__
#define __TAR_H__

#include <core/libk/stdint.h>

struct tar_header
{
    u8 filename[100];
    u8 mode[8];
    u8 UID[8];
    u8 GID[8];
    u8 size[12];
    u8 mtime[12];
    u8 chksum[8];
    u8 typeflag;
};

struct tar_contents
{
    u8 hdr_num;
    struct tar_header** headers;
};
u32 parse_size(u8 *in);
struct tar_header* find_file(struct tar_contents* findin,char* name);
struct tar_contents parse_tar(struct tar_header* header,u64 file_size);


#endif // __TAR_H__