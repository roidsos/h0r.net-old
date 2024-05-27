#include <core/kernel.h>
#include <core/sys/resman/SIV.h>
#include <libk/macros.h>
#include <libk/stdbool.h>
#include <libk/stdint.h>
#include <libk/string.h>
#include <libk/tar.h>
#include <utils/log.h>

u16 drive_id = 0;
struct tar_contents contents;

// Driver functions
file_t tar_get_props(UNUSED void *driver_specific_data, char *path) {
    struct tar_header *header = find_file(&contents, path);
    if (header == NULL) {
        return (file_t){0};
    }
    return (file_t){
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
void tar_read(UNUSED void *driver_specific_data, char *path, u32 offset,
              char *buffer, u32 size) {
    struct tar_header *thdr = find_file(&contents, path);
    usize fsize = parse_size(thdr->size);
    memcpy(buffer, ((char *)thdr) + 512 + offset, size > fsize ? fsize : size);
}
void tar_write(UNUSED void *driver_specific_data, char *path, u32 offset,
               char *buffer, u32 size) {
    struct tar_header *thdr = find_file(&contents, path);
    usize fsize = parse_size(thdr->size);
    memcpy(((char *)thdr) + 512 + offset, buffer, size > fsize ? fsize : size);
}

// Stubs
void tar_mkfile(UNUSED void *driver_specific_data, UNUSED char *path,
                UNUSED _bool isdir, UNUSED _bool recursive) {}
void tar_mklink(UNUSED void *driver_specific_data, UNUSED char *from,
                UNUSED char *to) {}
void tar_remove(UNUSED void *driver_specific_data, UNUSED char *path) {}
void tar_copy(UNUSED void *driver_specific_data, UNUSED char *from,
              UNUSED char *to) {}
void tar_move(UNUSED void *driver_specific_data, UNUSED char *from,
              UNUSED char *to) {}
void tar_chmod(UNUSED void *driver_specific_data, UNUSED char *path,
               UNUSED u8 perms, UNUSED u8 who) {}
void tar_chown(UNUSED void *driver_specific_data, UNUSED char *path,
               UNUSED u64 uid, UNUSED u64 gid) {}

void tar_init() {
    if (hn_data.initramfs == NULL) {
        return;
    }

    // WARN: Arch specific code in drivers/ + TODO: smh move to arch/ or make
    // architecture neutral
    contents = parse_tar(hn_data.initramfs->address, hn_data.initramfs->size);

    u16 driver_id =
        siv_register_driver((block_driver_t){.is_virtual = true,
                                             .get_props = tar_get_props,
                                             .chmod = tar_chmod,
                                             .chown = tar_chown,
                                             .read = tar_read,
                                             .write = tar_write,
                                             .mkfile = tar_mkfile,
                                             .mklink = tar_mklink,
                                             .remove = tar_remove,
                                             .copy = tar_copy,
                                             .move = tar_move});
    drive_id = siv_register_drive((siv_drive_t){.sig = SIV_SIG_INITRD,
                                                .driver_id = driver_id,
                                                .driver_specific_data = NULL});
}