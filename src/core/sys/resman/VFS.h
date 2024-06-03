#ifndef __VFS_H__
#define __VFS_H__

#include <libk/stdint.h>
#include <libk/stdbool.h>

//Signatures of physical drive types
#define VFS_SIG_HARDDRIVE    "HARD"
#define VFS_SIG_FLOPPY_DRIVE "FLOP"
#define VFS_SIG_CD_DRIVE     "CDVD"
#define VFS_SIG_USB_DRIVE    "USBV"

//Signatures of virtual drive types
#define VFS_SIG_INITRD       "INRD"
#define VFS_SIG_STDIO        "STIO"
#define VFS_SIG_STDERR       "STER"
#define VFS_SIG_REGISTERY    "REGS"

#define VFS_FLAGS_CACHE     1 << 0
#define VFS_FLAGS_FORCE_PRELOAD   1 << 1

#define VFS_PERM_OWNER_READ   1 << 0
#define VFS_PERM_OWNER_WRITE  1 << 1
#define VFS_PERM_OWNER_EXEC   1 << 2
#define VFS_PERM_OWNER_SEE    1 << 3

#define VFS_PERM_GROUP_READ   1 << 4
#define VFS_PERM_GROUP_WRITE  1 << 5
#define VFS_PERM_GROUP_EXEC   1 << 6
#define VFS_PERM_GROUP_SEE    1 << 7

#define VFS_PERM_OTHER_READ   1 << 8
#define VFS_PERM_OTHER_WRITE  1 << 9
#define VFS_PERM_OTHER_EXEC   1 << 10
#define VFS_PERM_OTHER_SEE    1 << 11

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
} file_t;

typedef struct {
    _bool is_virtual;
    // File functions
    file_t (*get_props)(void* driver_specific_data,char* path);
    void   (*chmod    )(void* driver_specific_data,char* path,u8 perms,u8 who);
    void   (*chown    )(void* driver_specific_data,char* path,u64 uid,u64 gid);
    void   (*read     )(void* driver_specific_data,char* path,u32 offset,char* buffer,u32 size);
    void   (*write    )(void* driver_specific_data,char* path,u32 offset,char* buffer,u32 size);

    // FS functions
    void   (*mkfile   )(void* driver_specific_data,char* path,_bool isdir,_bool recursive);
    void   (*mklink   )(void* driver_specific_data,char* from,char* to);
    void   (*remove   )(void* driver_specific_data,char* path);
    void   (*copy     )(void* driver_specific_data,char* from,char* to);
    void   (*move     )(void* driver_specific_data,char* from,char* to);
} block_driver_t;

typedef struct {
    char sig[4];
    u16 driver_id;
    void* driver_specific_data;
} vfs_drive_t;

void vfs_init();
u16 vfs_register_drive(vfs_drive_t drive);
u16 vfs_register_driver(block_driver_t driver);

u32 vfs_open(u32 drive_id,char* path,u8 flags);
void vfs_close(u32 file_desc);

file_t vfs_get_file(u32 file_desc);

void vfs_read(u32 file_desc, u32 offset, char* buf, u32 size);
void vfs_write(u32 file_desc, u32 offset, char* data, u32 size);

extern u16 vfs_num_drives;
extern u16 vfs_num_drivers;
extern u32 vfs_num_open_files;

#endif