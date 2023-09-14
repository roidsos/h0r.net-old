#ifndef __DRIVE_MANAGER_H__
#define __DRIVE_MANAGER_H__

#include <types/stdtypes.h>
#include "defines.h"
struct fs_driver
{
    bool present;
    struct node* (*get_file_header  )(char* path);
    void*        (*get_file_contents)(char* path);
};

void fs_driver_register(struct fs_driver driver);
struct node* get_file_header(int device_id,char* path);
void*        get_file_contents(int device_id,char* path);

#endif // __DRIVE_MANAGER_H__