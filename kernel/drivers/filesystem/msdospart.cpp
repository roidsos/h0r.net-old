#include <drivers/filesystem/fat.h>
#include <drivers/filesystem/msdospart.h>
#include <util/printf.h>
void LoadMBR(ATA ata){

    MasterBootRecord mbr;
    
    printf("MBR: ");
    
    ata.Read28(0, (uint_8*)&mbr, sizeof(MasterBootRecord));
    

    
    //for(int i = 0; i <= 0x01FF; i++)
    //{
    //    printf("0x%x ",((uint_8*)&mbr)[i]);
    //}
    //printf("\n");
    //return;
    
    
    if(mbr.magicnumber != 0xAA55)
    {
        printf("illegal MBR");
        return;
    }

    
    for(int i = 0; i < 4; i++)
    {
        if(mbr.primaryPartition[i].partition_id == 0x00){
            continue;
            printf("nope.\n");
        }
        printf(" Partition ");
        printf(" 0x %x",i & 0xFF);
        
        
        if(mbr.primaryPartition[i].bootable == 0x80)
            printf(" bootable. Type");
        else
            printf(" not bootable. Type ");
        
        
        printf(" 0x %x \n",mbr.primaryPartition[i].partition_id);
        
     
        LoadFAT(ata, mbr.primaryPartition[i].start_lba);
    }
}