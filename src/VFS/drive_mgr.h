#ifndef __DRIVE_MGR_H__
#define __DRIVE_MGR_H__
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "defines.h"
struct fs_driver
{
    bool                present;
    struct node         (*inspect      )(char* path);
    struct dir_report   (*iterate_dir  )(char* path);
    struct file_buffer  (*read_file    )(char* path,uint64_t offset,uint64_t size);
    bool                (*write_file   )(char* path,uint64_t offset,struct file_buffer buf);
};

size_t              fs_driver_register(struct fs_driver driver);
struct node         inspect(uint32_t device_id,char* path);
struct dir_report   iterate_dir(uint32_t device_id,char* path);
struct file_buffer  read_file(uint32_t device_id,char* path,uint64_t offset,uint64_t size);
bool                write_file(uint32_t device_id,char* path,uint64_t offset,struct file_buffer buf);

#endif // __DRIVE_MGR_H__