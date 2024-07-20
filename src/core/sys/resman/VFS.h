#ifndef __VFS_H__
#define __VFS_H__

#include <core/libk/stdint.h>
#include <core/libk/stdbool.h>
#include "devman.h"
#include "file.h"

#define VFS_FLAGS_CACHE         1 << 0
#define VFS_FLAGS_FORCE_PRELOAD 1 << 1

u32 vfs_open(u32 dev_id,char* path,u8 flags);
void vfs_close(u32 file_desc);

file_props_t vfs_get_file(u32 file_desc);

void vfs_read(u32 file_desc, u32 offset, char* buf, u32 size);
void vfs_write(u32 file_desc, u32 offset, char* data, u32 size);
#endif