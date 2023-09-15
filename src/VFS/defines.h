#ifndef __DEFINES_H__
#define __DEFINES_H__
#include <types/stdtypes.h>

#define FLAGS_PRESENT 1
#define FLAGS_ISDIR 1 << 1

struct node
{
    char* name;
    uint8_t flags;

    uint8_t ext[22];
};
struct ext_file
{
    uint8_t owner_user;
    uint8_t owner_group;
    uint64_t creation_date_unix_time;
    uint64_t last_modified_unix_time;
    uint16_t disk_id;
    uint64_t contents_on_disk_ptr;
    
    uint8_t filler[2];
};
struct ext_dir
{
    uint8_t owner_user;
    uint8_t owner_group;
    uint64_t creation_date_unix_time;
    uint64_t last_modified_unix_time;
    uint16_t disk_id;
    uint16_t num_dirs;
    struct node* files;

};

struct ext_link{
    struct node* linkto;
    uint8_t filler[14];
};


#endif // __DEFINES_H__