#ifndef __VFS_H__
#define __VFS_H__

#include <libk/stdint.h>
#include <libk/stdbool.h>
#include "file.h"

#define VFS_FLAGS_CACHE         1 << 0
#define VFS_FLAGS_FORCE_PRELOAD 1 << 1

void vfs_init();

u32 vfs_open(u32 drive_id,char* path,u8 flags);
void vfs_close(u32 file_desc);

file_t vfs_get_file(u32 file_desc);

void vfs_read(u32 file_desc, u32 offset, char* buf, u32 size);
void vfs_write(u32 file_desc, u32 offset, char* data, u32 size);

extern u16 vfs_num_drives;
extern u16 vfs_num_drivers;
extern u32 vfs_num_open_files;

#endif