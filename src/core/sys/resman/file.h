#ifndef __FILE_H__
#define __FILE_H__

#include <core/libk/stdint.h>
#include <core/libk/stdbool.h>

#define FILE_PERM_OWNER_READ   1 << 0
#define FILE_PERM_OWNER_WRITE  1 << 1
#define FILE_PERM_OWNER_EXEC   1 << 2
#define FILE_PERM_OWNER_SEE    1 << 3
#define FILE_PERM_GROUP_READ   1 << 4
#define FILE_PERM_GROUP_WRITE  1 << 5
#define FILE_PERM_GROUP_EXEC   1 << 6
#define FILE_PERM_GROUP_SEE    1 << 7
#define FILE_PERM_OTHER_READ   1 << 8
#define FILE_PERM_OTHER_WRITE  1 << 9
#define FILE_PERM_OTHER_EXEC   1 << 10
#define FILE_PERM_OTHER_SEE    1 << 11
#define FILE_PERM_FLAG_NO_SU   1 << 12

typedef struct{
    // Basic info
    u16 drive_id;
    char* name;
    char* full_path;
    u64 size;
    _bool isdir;

    // Time stuffs
    u64 created;
    u64 last_modified;
    u64 last_accessed;

    // security stuffs
    u64 owner_id;
    u64 group_id;

    u16 perms;
} file_props_t;

#endif // __FILE_H__