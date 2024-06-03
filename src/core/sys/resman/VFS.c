#include "VFS.h"
#include <config.h>
#include <core/mm/heap.h>
#include <libk/binary.h>
#include <libk/limits.h>
#include <libk/stddef.h>
#include <libk/stdint.h>
#include <libk/string.h>
#include <utils/log.h>

vfs_drive_t vfs_drives[MAX_DRIVES];
u16 vfs_num_drives = 0;

block_driver_t vfs_drivers[MAX_FS_DRIVERS];
u16 vfs_num_drivers = 0;

file_t open_files[MAX_OPEN_FILES];
char *open_file_contents[MAX_OPEN_FILES];
u32 vfs_num_open_files = 0;

void vfs_init() {
    memset(vfs_drives, 0, sizeof(vfs_drives));
    memset(vfs_drivers, 0, sizeof(vfs_drivers));
}

u16 vfs_register_drive(vfs_drive_t drive) {
    vfs_drives[vfs_num_drives] = drive;
    vfs_num_drives++;
    return vfs_num_drives - 1;
}

u16 vfs_register_driver(block_driver_t driver) {
    vfs_drivers[vfs_num_drivers] = driver;
    vfs_num_drivers++;
    return vfs_num_drivers - 1;
}

u32 vfs_open(u32 drive_id, char *path, u8 flags) {
    log_trace("vfs_open(%d, %s, %d)\n", drive_id, path, flags);
    vfs_drive_t drive = vfs_drives[drive_id];
    block_driver_t driver = vfs_drivers[drive.driver_id];

    u32 file_desc = vfs_num_open_files;
    vfs_num_open_files++;

    open_files[file_desc] = driver.get_props(drive.driver_specific_data, path);

    // TODO: check user perms
    if (open_files[file_desc].size == 0 &&
        open_files[file_desc].full_path == NULL) {
        goto error;
    }

    if (open_files[file_desc].isdir) {
        // this is not how you open directories lmao
        goto error;
    }

    if ((!driver.is_virtual && FLAG_READ(flags, VFS_FLAGS_CACHE)) ||
        FLAG_READ(flags, VFS_FLAGS_FORCE_PRELOAD)) {
        log_trace("Preloading file %s\n", path);
        open_file_contents[file_desc] = malloc(open_files[file_desc].size);
        driver.read(drive.driver_specific_data, path, 0,
                    open_file_contents[file_desc], open_files[file_desc].size);
    }

    return file_desc;

error:
    open_files[file_desc] = (file_t){0};
    return UINT32_MAX;
}

void vfs_close(u32 file_desc) {
    log_trace("vfs_close(%d)\n", file_desc);
    // TODO: check user perms
    if (open_file_contents[file_desc] != NULL) {
        free(open_file_contents[file_desc]);
        open_file_contents[file_desc] = NULL;
    }

    open_files[file_desc] = (file_t){0};
}
file_t vfs_get_file(u32 file_desc) { return open_files[file_desc]; }

void vfs_read(u32 file_desc, u32 offset, char *buf, u32 size) {
    log_trace("vfs_read(%d, %d, %d)\n", file_desc, offset, size);
    // TODO: check user perms
    if (open_file_contents[file_desc] != NULL) {
        memcpy(buf, open_file_contents[file_desc] + offset, size);
    } else {
        u16 drive_id = open_files[file_desc].drive_id;
        vfs_drivers[vfs_drives[drive_id].driver_id].read(
            vfs_drives[drive_id].driver_specific_data,
            open_files[file_desc].full_path, offset, buf, size);
    }
}

void vfs_write(u32 file_desc, u32 offset, char *data, u32 size) {
    log_trace("vfs_write(%d, %d, %d)\n", file_desc, offset, size);
    u16 drive_id = open_files[file_desc].drive_id;
    vfs_drivers[vfs_drives[drive_id].driver_id].write(
        vfs_drives[drive_id].driver_specific_data,
        open_files[file_desc].full_path, offset, data, size);
}