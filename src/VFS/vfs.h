#ifndef __VFS_H__
#define __VFS_H__
#include "defines.h"
#include "stdbool.h"

bool vfs_is_dir(char* path);
bool vfs_mount(uint16_t disk_id,char* path);
bool vfs_load_contents(struct node* node,char* path);
struct node* vfs_inspect(char* path);
struct dir_report vfs_iterate_dir(char* path);
struct file_buffer vfs_read(char* path,uint64_t offset,uint64_t size);
void vfs_write(char* path,struct file_buffer buf,uint64_t offset);

#endif // __VFS_H__