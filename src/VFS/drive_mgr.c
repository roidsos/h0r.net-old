#include "drive_mgr.h"

struct fs_driver drives[16];

struct file_buffer read_file(uint32_t device_id, char *path,uint64_t offset,uint64_t size) {
    if (!drives[device_id].present) {
        return (struct file_buffer){0,0};
    }
    return drives[device_id].read_file(path,offset,size);
}

bool write_file(uint32_t device_id,char* path,uint64_t offset,struct file_buffer buf)
{
    if (!drives[device_id].present) {
        return 0;
    }
    return drives[device_id].write_file(path,offset,buf);
}
struct dir_report iterate_dir(uint32_t device_id, char *path) {
    if (!drives[device_id].present) {
        return (struct dir_report){0,0};
    }
    return drives[device_id].iterate_dir(path);
}

struct node *inspect(uint32_t device_id, char *path) {
    if (!drives[device_id].present) {
        return 0;
    }
    return drives[device_id].inspect(path);
}

bool fs_driver_register(struct fs_driver driver) {
    for (size_t i = 0; i < 16; i++) {
        if (drives[i].present)
            continue;
        drives[i] = driver;
        return true;
    }
    return false;
}