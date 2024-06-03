#include "core/sys/resman/devman.h"
#include "core/sys/resman/file.h"
#include <core/kernel.h>
#include <core/sys/resman/VFS.h>
#include <libk/macros.h>
#include <libk/stdbool.h>
#include <libk/stdint.h>
#include <libk/string.h>
#include <libk/tar.h>
#include <utils/log.h>

u16 drive_id = 0;
struct tar_contents contents;

// Driver functions
file_props_t tar_get_props(UNUSED void *driver_specific_data, char *path) {
    struct tar_header *header = find_file(&contents, path);
    if (header == NULL) {
        return (file_props_t){0};
    }
    return (file_props_t){
        .drive_id = drive_id,
        .name = "",
        .full_path = path,
        .size = parse_size(header->size),
        .isdir = header->typeflag == '5',
        .created = 0,
        .last_modified = parse_size(header->mtime),
        .last_accessed = 0,
        .owner_id = parse_size(header->UID),
        .group_id = parse_size(header->GID),
        .perms = 0b0000111111111111, // anarchy, lol
    };
}
_bool tar_read(UNUSED void *driver_specific_data, char *path, u32 offset,
              char *buffer, u32 size) {
    struct tar_header *thdr = find_file(&contents, path);
    usize fsize = parse_size(thdr->size);
    memcpy(buffer, ((char *)thdr) + 512 + offset, size > fsize ? fsize : size);
    return true;
}
_bool tar_write(UNUSED void *driver_specific_data, char *path, u32 offset,
               char *buffer, u32 size) {
    struct tar_header *thdr = find_file(&contents, path);
    usize fsize = parse_size(thdr->size);
    memcpy(((char *)thdr) + 512 + offset, buffer, size > fsize ? fsize : size);
    return true;
}

// Stubs
_bool tar_mkfile(UNUSED void *driver_specific_data, UNUSED char *path,
                UNUSED _bool isdir, UNUSED _bool recursive) {
    return false;
}
_bool tar_mklink(UNUSED void *driver_specific_data, UNUSED char *from,
                UNUSED char *to) {
    return false;
}
_bool tar_remove(UNUSED void *driver_specific_data, UNUSED char *path) {
    return false;
}
_bool tar_copy(UNUSED void *driver_specific_data, UNUSED char *from,
              UNUSED char *to) {
    return false;
}
_bool tar_move(UNUSED void *driver_specific_data, UNUSED char *from,
              UNUSED char *to) {
    return false;
}
_bool tar_chmod(UNUSED void *driver_specific_data, UNUSED char *path,
               UNUSED u8 perms, UNUSED u8 who) {
    return false;
}
_bool tar_chown(UNUSED void *driver_specific_data, UNUSED char *path,
               UNUSED u8 uid, UNUSED u8 gid) {
    return false;
}

void tar_init() {
    if (hn_data.initramfs == NULL) {
        return;
    }

    // WARN: Arch specific code in drivers/ + TODO: smh move to arch/ or make
    // architecture neutral
    contents = parse_tar(hn_data.initramfs->address, hn_data.initramfs->size);

    drive_id = register_device((device_t){.PP = 0,
                                   .type = DEVICE_TYPE_FS,
                                   .u.fsdev.is_virtual = true,
                                   .u.fsdev.sig = FS_DEV_SIG_INITRD,
                                   .u.fsdev.id = tar_get_props,
                                   .u.fsdev.rd = tar_read,
                                   .u.fsdev.wr = tar_write,
                                   .u.fsdev.chp = tar_chmod,
                                   .u.fsdev.cho = tar_chown,
                                   .u.fsdev.mak = tar_mkfile,
                                   .u.fsdev.lnk = tar_mklink,
                                   .u.fsdev.rem = tar_remove,
                                   .u.fsdev.cpy = tar_copy,
                                   .u.fsdev.mov = tar_move});
}