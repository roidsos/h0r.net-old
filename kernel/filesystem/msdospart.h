#ifndef __MSDOSPART_H__
#define __MSDOSPART_H__

        struct PartitionTableEntry
        {
            uint_8 bootable;

            uint_8 start_head;
            uint_8 start_sector : 6;
            uint_16 start_cylinder : 10;

            uint_8 partition_id;

            uint_8 end_head;
            uint_8 end_sector : 6;
            uint_16 end_cylinder : 10;
            
            uint_32 start_lba;
            uint_32 length;
        } __attribute__((packed));
        

        struct MasterBootRecord
        {
            uint_8 bootloader[440];
            uint_32 signature;
            uint_16 unused;
            
            PartitionTableEntry primaryPartition[4];
            
            uint_16 magicnumber;
        } __attribute__((packed));

        void LoadMBR(ATA ata);

#endif // __MSDOSPART_H__