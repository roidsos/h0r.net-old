#ifndef __DEFINES_H__
#define __DEFINES_H__
#include <types/stdtypes.h>

struct node
{
    char* name;
    uint8_t flags;

    void* ext;
};
struct ext_file
{
    uint8_t owner_user;
    uint8_t owner_group;
    uint64_t creation_date_unix_time;
    uint64_t last_modified_unix_time;
    uint16_t disk_id;
    uint64_t contents_on_disk_ptr;
    
};
struct ext_dir
{
    uint8_t owner_user;
    uint8_t owner_group;
    uint64_t creation_date_unix_time;
    uint64_t last_modified_unix_time;
    uint16_t disk_id;
    uint64_t contents_on_disk_ptr;
    node* files;
};

struct ext_link{
    node* linkto;
};


#endif // __DEFINES_H__