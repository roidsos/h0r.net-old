#ifndef __VFS_H__
#define __VFS_H__

#include "drive_manager.h"

void vfs_init();
struct node* get_file(char* path);

#endif // __VFS_H__