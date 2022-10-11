#include <drivers/ata.h>
#include <util/printf.h>
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
        devs[0] = ATAdevice(4,true);
        devs[1] = ATAdevice(4,true);
        devs[2] = ATAdevice(4,true);
        devs[3] = ATAdevice(4,true);
        devs[4] = ATAdevice(4,true);
        devs[5] = ATAdevice(4,true);
        devs[6] = ATAdevice(4,true);
        devs[7] = ATAdevice(4,true);
        devs[8] = ATAdevice(4,true);
        devs[9] = ATAdevice(4,true);
        devs[10] = ATAdevice(4,true);
        devs[11] = ATAdevice(4,true);
        devs[12] = ATAdevice(4,true);
        devs[13] = ATAdevice(4,true);
        devs[14] = ATAdevice(4,true);
        devs[15] = ATAdevice(4,true);
        devs[16] = ATAdevice(4,true);
    }
    
    void Read28(uint_8 drive_num,uint_32 sector,uint_8* data,int count)
    {
        
    }
    
    void Write28(uint_8 drive_num,uint_32 sector,uint_8* data,int count)
    {
        
    }
    
    void ListDevices()
    {
        for (size_t i = 0; i < 16; i++)
        {
            printf("Device #%i(port #%i %s)\n",i,devs[i].portbase,devs[i].master ? "master" : "slave");
            
        }
        
    }
}