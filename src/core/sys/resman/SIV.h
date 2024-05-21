#ifndef __SIV_H__
#define __SIV_H__

#include <libk/stdint.h>
#include <libk/stdbool.h>

//Signatures of physical drive types
#define SIV_SIG_HARDDRIVE    "HARD"
#define SIV_SIG_FLOPPY_DRIVE "FLOP"
#define SIV_SIG_CD_DRIVE     "CDVD"
#define SIV_SIG_USB_DRIVE    "USBV"

//Signatures of virtual drive types
#define SIV_SIG_INITRD       "INRD"
#define SIV_SIG_STDIO        "STIO"
#define SIV_SIG_STDERR       "STER"
#define SIV_SIG_REGISTERY    "REGS"

#define SIV_FLAGS_CACHE     1 << 0
#define SIV_FLAGS_FORCE_PRELOAD   1 << 1

#define SIV_PERM_OWNER_READ   1 << 0
#define SIV_PERM_OWNER_WRITE  1 << 1
#define SIV_PERM_OWNER_EXEC   1 << 2
#define SIV_PERM_OWNER_SEE    1 << 3

#define SIV_PERM_GROUP_READ   1 << 4
#define SIV_PERM_GROUP_WRITE  1 << 5
#define SIV_PERM_GROUP_EXEC   1 << 6
#define SIV_PERM_GROUP_SEE    1 << 7

#define SIV_PERM_OTHER_READ   1 << 8
#define SIV_PERM_OTHER_WRITE  1 << 9
#define SIV_PERM_OTHER_EXEC   1 << 10
#define SIV_PERM_OTHER_SEE    1 << 11

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
} siv_drive_t;

void siv_init();
u16 siv_register_drive(siv_drive_t drive);
u16 siv_register_driver(block_driver_t driver);

u32 siv_open(u32 drive_id,char* path,u8 flags);
void siv_close(u32 file_desc);

file_t siv_get_file(u32 file_desc);

void siv_read(u32 file_desc, u32 offset, char* buf, u32 size);
void siv_write(u32 file_desc, u32 offset, char* data, u32 size);

extern u16 siv_num_drives;
extern u16 siv_num_drivers;
extern u32 siv_num_open_files;

#endif