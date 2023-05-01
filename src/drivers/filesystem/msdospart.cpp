#include <drivers/filesystem/fat.h>
#include <drivers/filesystem/msdospart.h>
#include <drivers/mass-storage.h>
#include <util/logger.h>
#include <lib/printf.h>
void LoadMBR(){

    MasterBootRecord mbr;
    
    mass_storage_manager::Read28(0,0, (uint_8*)&mbr, 1);
    

    
    //for(int i = 0; i <= 0x01FF; i++)
    //{
    //    printf("0x%x ",((uint_8*)&mbr)[i]);
    //}
    //printf("\n");
    //return;
    
    
    if(mbr.magicnumber != 0xAA55)
    {
        LogERR("MBR: illegal MBR\n");
        return;
    }

    LogINFO("MBR:\n");
    for(int i = 0; i < 4; i++)
    {
        if(mbr.primaryPartition[i].partition_id == 0x00){
            continue;
        }
        LogINFO(" Partition 0x%x %s",i & 0xFF,mbr.primaryPartition[i].bootable == 0x80 ?" bootable. Type" : " not bootable. Type ");
        
        //printf(" 0x %x \n",mbr.primaryPartition[i].partition_id);
        //printf("startlba: %i \n",mbr.primaryPartition[i].start_lba);
        LoadFAT(mbr.primaryPartition[i].start_lba);
    }
}