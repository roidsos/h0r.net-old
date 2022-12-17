#include <drivers/ata.h>
#include <lib/printf.h>

#include <drivers/ataDefs.h>

ATAdevice::ATAdevice(uint_16 _portbase,bool _master)
{
    BytesPerSector = 512;
    portbase = _portbase;
    master = _master;
}
ATAdevice::ATAdevice()
{
    BytesPerSector = 512;
    portbase = 0;
    master = false;
}

void ATAdevice::Identify()
{
    outb8(portbase + 6,master ? 0xA0 : 0xB0);//switching to the correct device
    outb8(portbase + 0x206,0);

    outb8(portbase + 6,0xA0);//switching to the master
    register uint_8 status = inb8(portbase + 7);
    if(status == 255){
        print("[Error] No device found.\n");
        return;// no device 
    }
    outb8(portbase + 6,master ? 0xA0 : 0xB0);//switching to the correct device
    outb8(portbase + 2,0);// loading the device with zeroes
    outb8(portbase + 3,0);
    outb8(portbase + 5,0);
    outb8(portbase + 4,0);
    outb8(portbase + 7,0xEC);// specifying the command: 0xEC - identify
    
    status = inb8(portbase + 7);
    if(status == 0){
        print("[Error] No device found.\n");
        return;// no device
    }
    while(((status & 0x80) == 0x80)&& (status & 0x01) != 0x01){
        status = inb8(portbase + 7);
    }

    if (status & 0x01){
        print("ERROR\n");
        return;
    }

    for(uint_16 i = 0;i < 256;i++){
        uint_16 data = inb16(portbase);
        printf("0x%x",data);
    }
    
}

void ATAdevice::RW28(uint_32 sector,uint_8* data,int count,bool write)
{
    //code modified, but taken from https://github.com/ilobilo/kernel/blob/master/source/drivers/block/ata/ata.cpp#L41
    uint_8 status = inb8(ATA_REGISTER_STATUS);
    if (status == 0xFF)
        return;

    outb8(ATA_REGISTER_DRIVE_HEAD, 0xE0 | ((sector >> 24 )& 0x0F));// setting the head
    // Explanation for above:
    // bits 0-3 = ((sector & 0x0F000000) >> 24)
    // bit  4   = drive number
    // bit  5   = always 1
    // bit  6   = Uses CHS addressing if clear or LBA addressing if set. 
    // bit  7   = always 1

    for (size_t i = 0; i < 4; i++) inb8(ATA_REGISTER_DATA);
    while (inb8(ATA_REGISTER_STATUS) & ATA_DEV_BUSY);

    outb8(ATA_REGISTER_SECTOR_COUNT, count);

    outb8(ATA_REGISTER_LBA_LOW, sector & 0xFF);
    outb8(ATA_REGISTER_LBA_MID, (sector >> 8) & 0xFF);
    outb8(ATA_REGISTER_LBA_HIGH, (sector >> 16) & 0xFF);

    while (inb8(ATA_REGISTER_STATUS) & ATA_DEV_BUSY || !(inb8(ATA_REGISTER_STATUS) & ATA_DEV_DRDY));

    outb8(ATA_REGISTER_COMMAND, (write ? ATA_CMD_WRITE : ATA_CMD_READ));

    status = inb8(ATA_REGISTER_STATUS);
    while (status & ATA_DEV_BUSY)
    {
        if (status & ATA_DEV_ERR)
        {
            printf("ATA: %s error!", write ? "write" : "read");
            return;
        }
        status = inb8(ATA_REGISTER_STATUS);
    }
    if (write){
    for(uint_16 i = 0; i < count; i+= 2){
        uint_16 wdata = data[i] << 8;        
        if(i+1 < count)
            wdata |= data[i+1];
        outb16(portbase,wdata);
    }
    }else{
            for(uint_16 i = 0; i < count; i+= 2){
          uint_16 wdata = inb16(portbase);
        data[i] = wdata & 0x00ff;
        if(i+1 < count)
            data[i+1] = (wdata >> 8) & 0x00FF;
    }
    }

}
void ATAdevice::Flush()
{

    outb8(portbase + 6,master ? 0xe0 : 0xf0);//switching to the correct device
    outb8(portbase + 7,0xe7);// specifying the command: 0xe7 - flush

    while(((inb8(portbase + 7) & 0x80) == 0x80)&& ((inb8(portbase + 7) & 0x01) != 0x01)){}


}

namespace ATA
{
    ATAdevice devs[16];
    void Init()
    {
        devs[0] = ATAdevice(0x1F0,false);
    }
    
    void Read28(uint_8 drive_num,uint_32 sector,uint_8* data,int count)
    {
        devs[drive_num].RW28(sector,data,count,false);
        devs[drive_num].Flush();
    }
    
    void Write28(uint_8 drive_num,uint_32 sector,uint_8* data,int count)
    {
        devs[drive_num].RW28(sector,data,count,true);
        devs[drive_num].Flush();
        
    }
    
    void ListDevices()
    {
        for (size_t i = 0; i < 16; i++)
        {
            if(devs[i].BytesPerSector == 512)
            printf("Device #%i(port #%i %s)\n",i,devs[i].portbase,devs[i].master ? "master" : "slave");
            
        }
        
    }
}