#ifndef __ATA_H__
#define __ATA_H__
#include <util/stdint.h>
#include <io/io.h>

struct ATAController{
   void (*RW_func)(int device,char direction,uint_8* destination,int address,int sector_count);
};

struct ATADevice{
    uint_16 parent_index;
    uint_16 index_inside_parent;
};

namespace mass_storage_manager
{
    uint_16 RegisterDevice(uint_16 parent_index,uint_16 index_inside_parent);
    uint_16 RegisterController(void (*RW_func)(int device,char direction,char* destination,int address,char sector_count));
    void Read28(uint_8 drive_num,uint_32 sector,uint_8* data,int count);
    void Write28(uint_8 drive_num,uint_32 sector,uint_8* data,int count);
    void ListDevices();
}

#endif 