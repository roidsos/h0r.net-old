#ifndef __DEVMAN_H__
#define __DEVMAN_H__

#include <libk/stdint.h>
#include <libk/stdbool.h>
#include <libk/stddef.h>

#define DEVICE_TYPE_BLOCK 0
#define DEVICE_TYPE_CHAR 1
#define DEVICE_TYPE_FS 2

#define FS_DEV_SIG_HARDDRIVE    "HARD"
#define FS_DEV_SIG_CD_DRIVE     "CDVD"
#define FS_DEV_SIG_USB_DRIVE    "USBV"

#define FS_DEV_SIG_INITRD       "INRD"
#define FS_DEV_SIG_STDIO        "STIO"
#define FS_DEV_SIG_STDERR       "STER"
#define FS_DEV_SIG_REGISTERY    "REGS"

typedef struct {
    u8 type;
    void* PP;
    union {
        struct {
            _bool (*read)(void* PP,u8* buf, u64 offset, u64 size);
            _bool (*write)(void* PP,u8* buf, u64 offset, u64 size);
        } blockdev;
        struct {
            h0r_char (*read)(void* PP);
            void (*write)(void* PP,h0r_char c); 
        } chardev;
        struct {
            char sig[4];
            _bool is_virtual;

            file_props_t (*id)(void* drive_specific_data,char* path);
            void         (*rd)(void* drive_specific_data,char* path,u32 offset,char* buffer,u32 size);
            void         (*wr)(void* drive_specific_data,char* path,u32 offset,char* buffer,u32 size);

            void         (*chp)(void* drive_specific_data,char* path,u8 perms,u8 who);
            void         (*cho)(void* drive_specific_data,char* path,u64 uid,u64 gid); 
            void         (*mak)(void* drive_specific_data,char* path,_bool isdir,_bool recursive);
            void         (*lnk)(void* drive_specific_data,char* from,char* to);
            void         (*rem)(void* drive_specific_data,char* path);
            void         (*cpy)(void* drive_specific_data,char* from,char* to);
            void         (*mov)(void* drive_specific_data,char* from,char* to);

            void* driver_specific_data;
        } fs_dev; 
    } u;
} device_t;
#endif // __DEVMAN_H__