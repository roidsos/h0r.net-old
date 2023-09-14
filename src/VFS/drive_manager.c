#include "drive_manager.h"

struct fs_driver drivers[16];

void* get_file_contents(int device_id,char* path)
{
    if(!drivers[device_id].present){
        return 0;
    }
    return drivers[device_id].get_file_contents(path);
}

struct node* get_file_header(int device_id,char* path)
{
    if(!drivers[device_id].present){
        return 0;
    }
    return drivers[device_id].get_file_header(path);
}

void fs_driver_register(struct fs_driver driver)
{
    for (size_t i = 0; i < 16; i++)
    {
        if(drivers[i].present) continue;
        drivers[i] = driver;
    }

}
