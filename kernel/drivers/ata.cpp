#include <drivers/ata.h>
ATA::ATA(uint_16 _portbase,bool _master)
{
    BytesPerSector = 512;
    portbase = _portbase;
    master = _master;
}

ATA::~ATA()
{
     
}

void ATA::Identify()
{
    outb8(portbase + 6,master ? 0xA0 : 0xB0);//switching to the correct device
    outb8(portbase + 0x206,0);

    outb8(portbase + 6,0xA0);//switching to the master
    if(inb8(portbase + 7) == 255){
        print("[Error] No device found.\n");
        return;// no device 
    }
    outb8(portbase + 6,master ? 0xA0 : 0xB0);//switching to the correct device
    outb8(portbase + 2,0);// loading the device with zeroes
    outb8(portbase + 3,0);
    outb8(portbase + 5,0);
    outb8(portbase + 4,0);
    outb8(portbase + 7,0xEC);// specifying the command: 0xEC - identify
    
    uint_8 status = inb8(portbase + 7);
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
        print(hex2str((uint_64)data));
    }
    
}

void ATA::Read28(uint_32 sector,uint_8* data,int count)
{
    if (count > BytesPerSector)
        return;
    if(sector & 0xf0000000)
        return;
    outb8(portbase + 6,master ? 0xe0 : 0xf0 |( sector & 0x0f000000) >> 24);//switching to the correct device
    outb8(portbase + 1,0);// clear errors
    outb8(portbase + 2,1);

    outb8(portbase + 3,(sector & 0x0000000ff));
    outb8(portbase + 5,(sector & 0x00000ff00) >> 8);
    outb8(portbase + 4,(sector & 0x000ff0000) >>16);
    outb8(portbase + 7,0x20);// specifying the command: 0x20 - read

    while(((inb8(portbase + 7) & 0x80) == 0x80)&& ((inb8(portbase + 7) & 0x01) != 0x01)){}

    for(uint_16 i = 0;i < count;i++){
        register uint_16 wdata = inb16(portbase);
        data[i] = wdata & 0x00ff;
        if (i+ 1 < count)
            data[i+1] = (wdata >> 8) & 0x00ff;
    }

    for(uint_16 i = count + count % 2;i < BytesPerSector;i++){
        inb16(portbase);
    }
    
    
}

void ATA::Write28(uint_32 sector,uint_8* data,int count)
{
    if (count > BytesPerSector)
        return;
    if(sector & 0xf0000000)
        return;
    outb8(portbase + 6,master ? 0xe0 : 0xf0 |( sector & 0x0f000000) >> 24);//switching to the correct device
    outb8(portbase + 1,0);// clear errors
    outb8(portbase + 2,1);

    outb8(portbase + 3,(sector & 0x0000000ff));
    outb8(portbase + 5,(sector & 0x00000ff00) >> 8);
    outb8(portbase + 4,(sector & 0x000ff0000) >>16);
    outb8(portbase + 7,0x30);// specifying the command: 0x30 - write

    for(uint_16 i = 0;i < count;i++){
        uint_16 wdata = data[i];
        if (i+ 1 < count)
            wdata |= ((uint_16)data[i+1] << 8 );
        outb16(portbase,wdata);
    }
    for(uint_16 i = count + count % 2;i < BytesPerSector;i++){
        outb16(portbase,0x0000);
    }
}

void ATA::Flush()
{

    outb8(portbase + 6,master ? 0xe0 : 0xf0);//switching to the correct device
    outb8(portbase + 7,0xe7);// specifying the command: 0xe7 - flush

    while(((inb8(portbase + 7) & 0x80) == 0x80)&& ((inb8(portbase + 7) & 0x01) != 0x01)){}

}
