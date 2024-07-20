#include "VFS.h"
#include "devman.h"
#include <config.h>
#include <core/mm/heap.h>
#include <core/libk/binary.h>
#include <core/libk/limits.h>
#include <core/libk/stddef.h>
#include <core/libk/stdint.h>
#include <core/libk/string.h>
#include <core/utils/log.h>

file_props_t open_files[MAX_OPEN_FILES] = {0};
char *open_file_contents[MAX_OPEN_FILES] = {0};
u32 vfs_num_open_files = 0;

u32 vfs_open(u32 dev_id, char *path, u8 flags) {
    log_trace("vfs_open(%d, %s, %d)\n", dev_id, path, flags);
    u32 file_desc = vfs_num_open_files;
    vfs_num_open_files++;

    open_files[file_desc] = fsdev_id(dev_id, path);

    // TODO: check user perms
    if (open_files[file_desc].size == 0 &&
        open_files[file_desc].full_path == NULL) {
        goto error;
    }

    if (open_files[file_desc].isdir) {
        // this is not how you open directories lmao
        goto error;
    }

    if ((!get_device_by_id(dev_id)->u.fsdev.is_virtual && FLAG_READ(flags, VFS_FLAGS_CACHE)) ||
        FLAG_READ(flags, VFS_FLAGS_FORCE_PRELOAD)) {
        log_trace("Preloading file %s\n", path);
        open_file_contents[file_desc] = malloc(open_files[file_desc].size);
        fsdev_rd(dev_id,path, 0, open_file_contents[file_desc], open_files[file_desc].size);
    }

    return file_desc;

error:
    open_files[file_desc] = (file_props_t){0};
    return UINT32_MAX;
}

void vfs_close(u32 file_desc) {
    log_trace("vfs_close(%d)\n", file_desc);
    // TODO: check user perms
    if (open_file_contents[file_desc] != NULL) {
        free(open_file_contents[file_desc]);
        open_file_contents[file_desc] = NULL;
    }

    open_files[file_desc] = (file_props_t){0};
}
file_props_t vfs_get_file(u32 file_desc) { return open_files[file_desc]; }

void vfs_read(u32 file_desc, u32 offset, char *buf, u32 size) {
    log_trace("vfs_read(%d, %d, %d)\n", file_desc, offset, size);
    // TODO: check user perms
    if (open_file_contents[file_desc] != NULL) {
        memcpy(buf, open_file_contents[file_desc] + offset, size);
    } else {
        fsdev_rd(open_files[file_desc].drive_id, open_files[file_desc].full_path, offset, buf, size);
    }
}

void vfs_write(u32 file_desc, u32 offset, char *data, u32 size) {
    log_trace("vfs_write(%d, %d, %d)\n", file_desc, offset, size);
    fsdev_wr(open_files[file_desc].drive_id, open_files[file_desc].full_path, offset, data, size);
}