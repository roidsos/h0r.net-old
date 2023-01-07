#include <drivers/mass-storage.h>
#include <drivers/memory/Heap.h>
#include <drivers/memory/memory.h>
#include <lib/printf.h>

namespace mass_storage_manager
{
    MSController* controllers = (MSController*)calloc(sizeof(MSController) * 255);
    MSDevice* devices = (MSDevice*)calloc(sizeof(MSDevice) * 4096);
    uint_16 controllerssize;
    uint_16 devicessize;


    uint_16 RegisterDevice(uint_16 parent_index,uint_16 index_inside_parent)
    {
        MSDevice dev;
        dev.parent_index = parent_index;
        dev.index_inside_parent = index_inside_parent;

        uint_16 index = controllerssize;
        devices[devicessize] = dev;
        devicessize++;
        return index;
        
    }

    uint_16 RegisterController(void (*RW_func)(int device,char direction,uint_8* destination,int address,int sector_count))
    {
        uint_16 index = controllerssize;
        controllers[controllerssize].RW_func = RW_func;
        controllerssize++;
        return index;
    }
    
    void Read28(uint_8 drive_num,uint_32 sector,uint_8* data,int count)
    {
        if (devicessize == 0){
            printf("ERROR:No Storage Devices Regisrtered\n");
            return;
        }
        uint_16 controller_index = devices[drive_num].parent_index + 1;
        printf("s: %i",(unsigned int)controllers[controller_index].RW_func);
        controllers[controller_index].RW_func(devices[drive_num].index_inside_parent,0,data,sector,count);
    }
    
    void Write28(uint_8 drive_num,uint_32 sector,uint_8* data,int count)
    {
        if (devicessize == 0){
            printf("ERROR:No Storage Devices Regisrtered\n");
            return;
        }        
        uint_16 controller_index = devices[drive_num].parent_index;
        controllers[controller_index].RW_func(devices[drive_num].index_inside_parent,1,data,sector,count);
    }
    
    void ListDevices()
    {
        printf("nope, I was too lazy to implement this important debug tool \n");
    }
}