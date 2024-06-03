#include "devman.h"
#include "config.h"

device_t devices[MAX_DEVICES] = {0};
u32 num_devices = 0;

u32 register_device(device_t device)
{
    if (num_devices < MAX_DEVICES) {
            devices[num_devices] = device;
            num_devices++;
            return num_devices - 1;
    }
    return -1;
}

device_t* get_device_by_id(u32 id)
{
    if (id < num_devices) {
        return &devices[id];
    }
    return NULL;
}

void remove_device(u32 id)
{
    devices[id].type = 0;
    num_devices--;
    for (u32 i = id; i < num_devices; i++) {
        devices[i] = devices[i + 1];
    }
}

_bool blockdev_read(u32 device_id, u8* buf, u64 offset, u64 size)
{
    if (device_id < num_devices) {
        if (devices[device_id].type != DEVICE_TYPE_BLOCK) return false;
        return devices[device_id].u.blockdev.read(devices[device_id].PP,buf, offset, size);
    }
    return false;
}

_bool blockdev_write(u32 device_id, u8* buf, u64 offset, u64 size)
{
    if (device_id < num_devices) {
        if (devices[device_id].type != DEVICE_TYPE_BLOCK) return false;
        return devices[device_id].u.blockdev.write(devices[device_id].PP,buf, offset, size);
    }
    return false;
}

h0r_char chardev_read(u32 device_id)
{
    if (device_id < num_devices) {
        if (devices[device_id].type != DEVICE_TYPE_CHAR) return 0;
        return devices[device_id].u.chardev.read(devices[device_id].PP);
    }
    return 0;
}

_bool chardev_write(u32 device_id, h0r_char c)
{
    if (device_id < num_devices) {
        if (devices[device_id].type != DEVICE_TYPE_CHAR) return false;
        return devices[device_id].u.chardev.write(devices[device_id].PP,c);
    }
    return false;
}

file_props_t fsdev_id(u32 device_id, char* path)
{
    if (device_id < num_devices) {
        if (devices[device_id].type != DEVICE_TYPE_FS) return (file_props_t){0};
        return devices[device_id].u.fsdev.id(devices[device_id].PP,path);
    }
    return (file_props_t){0};
}

_bool fsdev_rd(u32 device_id, char* path, u32 offset, char* buffer, u32 size)
{
    if (device_id < num_devices) {
        if (devices[device_id].type != DEVICE_TYPE_FS) return false;
        return devices[device_id].u.fsdev.rd(devices[device_id].PP,path, offset, buffer, size);
    }
    return false;
}

_bool fsdev_wr(u32 device_id, char* path, u32 offset, char* buffer, u32 size)
{
    if (device_id < num_devices) {
        if (devices[device_id].type != DEVICE_TYPE_FS) return false;
        return devices[device_id].u.fsdev.wr(devices[device_id].PP,path, offset, buffer, size);
    }
    return false;
}

_bool fsdev_chp(u32 device_id, char* path, u8 perms, u8 who)
{
    if (device_id < num_devices) {
        if (devices[device_id].type != DEVICE_TYPE_FS) return false;
        return devices[device_id].u.fsdev.chp(devices[device_id].PP,path, perms, who);
    }
    return false;
}

_bool fsdev_cho(u32 device_id, char* path, u64 uid, u64 gid)
{
    if (device_id < num_devices) {
        if (devices[device_id].type != DEVICE_TYPE_FS) return false;
        return devices[device_id].u.fsdev.cho(devices[device_id].PP,path, uid, gid);
    }
    return false;
}

_bool fsdev_mak(u32 device_id, char* path, _bool isdir, _bool recursive)
{
    if (device_id < num_devices) {
        if (devices[device_id].type != DEVICE_TYPE_FS) return false;
        return devices[device_id].u.fsdev.mak(devices[device_id].PP,path, isdir, recursive);
    }
    return false;
}

_bool fsdev_lnk(u32 device_id, char* from, char* to)
{
    if (device_id < num_devices) {
        if (devices[device_id].type != DEVICE_TYPE_FS) return false;
        return devices[device_id].u.fsdev.lnk(devices[device_id].PP,from, to);
    }
    return false;
}

_bool fsdev_rem(u32 device_id, char* path)
{
    if (device_id < num_devices) {
        if (devices[device_id].type != DEVICE_TYPE_FS) return false;
        return devices[device_id].u.fsdev.rem(devices[device_id].PP,path);
    }
    return false;
}

_bool fsdev_cpy(u32 device_id, char* from, char* to)
{
    if (device_id < num_devices) {
        if (devices[device_id].type != DEVICE_TYPE_FS) return false;
        return devices[device_id].u.fsdev.cpy(devices[device_id].PP,from, to);
    }
    return false;
}

_bool fsdev_mov(u32 device_id, char* from, char* to)
{
    if (device_id < num_devices) {
        if (devices[device_id].type != DEVICE_TYPE_FS) return false;
        return devices[device_id].u.fsdev.mov(devices[device_id].PP,from, to);
    }
    return false;
}
