#ifndef __ATA_H__
#define __ATA_H__
#include <stdint.h>
#include <io.h>
class ATA{
    public:
    /*
    dataport: 16bit     =   portbase
    errorport: 8bit     =   portbase + 1
    sectorport: 8bit    =   portbase + 2
    lbalowport: 8bit    =   portbase + 3
    lbahiport: 8bit     =   portbase + 4
    lbamidport: 8bit    =   portbase + 5
    deviceport: 8bit    =   portbase + 6
    commandport: 8bit   =   portbase + 7
    controlport: 8bit   =   portbase + 0x206
    */
   uint_16 portbase;
   bool master;
   uint_16 BytesPerSector;
   ATA(uint_16 _portbase,bool _master);
   ~ATA();

    void Identify();
    void Read28(uint_32 sector,uint_8* data,int count);
    void Write28(uint_32 sector,uint_8* data,int count);
    void Flush();
};

#endif 