#ifndef __DEFINES_H__
#define __DEFINES_H__
#include <stdint.h>
#include <stddef.h>

#define FLAGS_PRESENT 1
#define FLAGS_ISDIR 2
#define FLAGS_LOADED 4
#define FLAGS_MOUNT_POINT 8
struct dir_report{
    uint32_t num_entries;
    struct node* entries;  
};

struct ext_file
{
    uint8_t owner_user;
    uint8_t owner_group;
    uint64_t creation_date_unix_time;
    uint64_t last_modified_unix_time;
    uint16_t disk_id;
    char* contents_on_disk_path;
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
};

struct file_buffer{
    void*           data;
    size_t          size;
};

struct node
{
    char* name;
    uint8_t flags;

    uint8_t* ext;
};


#endif // __DEFINES_H__