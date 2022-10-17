#ifndef __ATA_H__
#define __ATA_H__
#include <util/stdint.h>
#include <io/io.h>



class ATAdevice{
    public:
   uint_16 portbase;
   bool master;
   uint_16 BytesPerSector;
   ATAdevice(uint_16 _portbase,bool _master);
   ATAdevice();

    void Identify();
    void RW28(uint_32 sector,uint_8* data,int count,bool write);
    void Flush();
};

namespace ATA
{
    void Init();
    void Read28(uint_8 drive_num,uint_32 sector,uint_8* data,int count);
    void Write28(uint_8 drive_num,uint_32 sector,uint_8* data,int count);
    void ListDevices();
}

#endif 