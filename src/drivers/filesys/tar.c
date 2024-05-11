#include <parsing/tar.h>
#include <core/sys/resman/SIV.h>
#include <stdint.h>
#include <klibc/string.h>
#include <utils/log.h>
#include <core/kernel.h>

uint16_t drive_id = 0;
struct tar_contents contents;

// Driver functions
file_t tar_get_props(UNUSED void* driver_specific_data,char* path){
    log_trace("tar_get_props(%s)\n", path);
    struct tar_header* header = find_file(&contents, path);
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
void tar_read(UNUSED void* driver_specific_data,char* path,uint32_t offset,char* buffer,uint32_t size){
    log_trace("tar_read(%s, %d, %d)\n", path, offset, size);
    struct tar_header *thdr = find_file(&contents, path);
    size_t fsize = parse_size(thdr->size);
    memcpy(buffer, ((char *)thdr) + 512 + offset, size > fsize ? fsize : size);
}
void tar_write(UNUSED void* driver_specific_data,char* path,uint32_t offset,char* buffer,uint32_t size){
    log_trace("tar_write(%s, %d, %d)\n", path, offset, size);
    struct tar_header *thdr = find_file(&contents, path);
    size_t fsize = parse_size(thdr->size);
    memcpy(((char *)thdr) + 512 + offset, buffer, size > fsize ? fsize : size);
}

// Stubs
void tar_mkfile(UNUSED void* driver_specific_data,UNUSED char* path,UNUSED bool isdir,UNUSED bool recursive){
    
}
void tar_mklink(UNUSED void* driver_specific_data,UNUSED char* from,UNUSED char* to){
    
}
void tar_remove(UNUSED void* driver_specific_data,UNUSED char* path){
    
}
void tar_copy(UNUSED void* driver_specific_data,UNUSED char* from,UNUSED char* to){
    
}
void tar_move(UNUSED void* driver_specific_data,UNUSED char* from,UNUSED char* to){
    
}
void tar_chmod(UNUSED void* driver_specific_data,UNUSED char* path,UNUSED uint8_t perms,UNUSED uint8_t who){
    
}
void tar_chown(UNUSED void* driver_specific_data,UNUSED char* path,UNUSED uint64_t uid,UNUSED uint64_t gid){
    
}

void tar_init() {
    if(data.initramfs == NULL) {
        return;
    }

    contents = parse_tar(data.initramfs->address,data.initramfs->size);

    block_driver_t driver = {
        .is_virtual = true,

        .get_props = tar_get_props,
        .chmod    = tar_chmod,
        .chown    = tar_chown,
        .read     = tar_read,
        .write    = tar_write,
        .mkfile   = tar_mkfile,
        .mklink   = tar_mklink,
        .remove   = tar_remove,
        .copy     = tar_copy,
        .move     = tar_move
    };

    uint16_t driver_id = siv_register_driver(driver);

    siv_drive_t drive = {
        .sig = SIV_SIG_INITRD,
        .driver_id = driver_id,
        .driver_specific_data = NULL
    };

    drive_id = siv_register_drive(drive);
}