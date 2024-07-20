#ifndef __DEVMAN_H__
#define __DEVMAN_H__

#include <core/libk/stdint.h>
#include <core/libk/stdbool.h>
#include <core/libk/stddef.h>
#include "file.h"

#define DEVICE_TYPE_NONE        0
#define DEVICE_TYPE_BLOCK       1
#define DEVICE_TYPE_CHAR        2
#define DEVICE_TYPE_FS          3

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
            _bool (*write)(void* PP,h0r_char c); 
        } chardev;
        struct {
            char sig[4];
            _bool is_virtual;

            file_props_t  (*id )(void* PP,char* path);
            _bool         (*rd )(void* PP,char* path,u32 offset,char* buffer,u32 size);
            _bool         (*wr )(void* PP,char* path,u32 offset,char* buffer,u32 size);

            _bool         (*chp)(void* PP,char* path,u8 perms,u8 who);
            _bool         (*cho)(void* PP,char* path,u8 uid,u8 gid);
            _bool         (*mak)(void* PP,char* path,_bool isdir,_bool recursive);
            _bool         (*lnk)(void* PP,char* from,char* to);
            _bool         (*rem)(void* PP,char* path);
            _bool         (*cpy)(void* PP,char* from,char* to);
            _bool         (*mov)(void* PP,char* from,char* to);

            void* driver_specific_data;
        } fsdev; 
    } u;
} device_t;

u32 register_device(device_t device);
device_t *get_device_by_id(u32 id);
void remove_device(u32 id);

_bool blockdev_read(u32 device_id, u8* buf, u64 offset, u64 size);
_bool blockdev_write(u32 device_id, u8* buf, u64 offset, u64 size);

h0r_char chardev_read(u32 device_id);
_bool chardev_write(u32 device_id, h0r_char c);

file_props_t fsdev_id(u32 device_id, char* path);
_bool fsdev_rd(u32 device_id, char* path, u32 offset, char* buffer, u32 size);
_bool fsdev_wr(u32 device_id, char* path, u32 offset, char* buffer, u32 size);

_bool fsdev_chp(u32 device_id, char* path, u8 perms, u8 who);
_bool fsdev_cho(u32 device_id, char* path, u64 uid, u64 gid);
_bool fsdev_mak(u32 device_id, char* path, _bool isdir, _bool recursive);
_bool fsdev_lnk(u32 device_id, char* from, char* to);
_bool fsdev_rem(u32 device_id, char* path);
_bool fsdev_cpy(u32 device_id, char* from, char* to);
_bool fsdev_mov(u32 device_id, char* from, char* to);

#endif // __DEVMAN_H__