// most of the code is copied from https://wiki.osdev.org/PCI_IDE_Controller
#include <drivers/pciide.h>
#include <drivers/PIT.h>
#include <drivers/mass-storage.h>
#include <drivers/memory/Heap.h>
#include <io/io.h>
#include <lib/printf.h>

//awful implementation sadly
uint_16 controller_index;
uint_16 devs_size;
ATADevice::ATADevice()
{
   mass_storage_manager::RegisterDevice(controller_index,devs_size);
}

namespace ATA{
   ATADevice devs[1024];

   static void init()
   {
      mass_storage_manager::RegisterController(&read_or_write);
      pciide controller(0x1F0, 0x3F6, 0x170, 0x376, 0x000);
   }
   
   void read_or_write(int device,char direction,uint_8* destination,int address,int sector_count)
   {
      pciide parent = *(devs[device].parent);
      parent.ide_read_slash_write(devs[device].index_inside_parent,direction,destination,address,sector_count);
   }

   
}
//end of the awful stuff
void ide_write(unsigned char channel, unsigned char reg, unsigned char data); // forward declaration

unsigned char pciide::ide_read(unsigned char channel, unsigned char reg) {
   unsigned char result;
   if (reg > 0x07 && reg < 0x0C)
      ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].nIEN);
   if (reg < 0x08)
      result = inb8(channels[channel].base + reg - 0x00);
   else if (reg < 0x0C)
      result = inb8(channels[channel].base  + reg - 0x06);
   else if (reg < 0x0E)
      result = inb8(channels[channel].ctrl  + reg - 0x0A);
   else if (reg < 0x16)
      result = inb8(channels[channel].bmide + reg - 0x0E);
   if (reg > 0x07 && reg < 0x0C)
      ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN);
   return result;
}

void pciide::ide_write(unsigned char channel, unsigned char reg, unsigned char data) {
   if (reg > 0x07 && reg < 0x0C)
      ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].nIEN);
   if (reg < 0x08)
      outb8(channels[channel].base  + reg - 0x00, data);
   else if (reg < 0x0C)
      outb8(channels[channel].base  + reg - 0x06, data);
   else if (reg < 0x0E)
      outb8(channels[channel].ctrl  + reg - 0x0A, data);
   else if (reg < 0x16)
      outb8(channels[channel].bmide + reg - 0x0E, data);
   if (reg > 0x07 && reg < 0x0C)
      ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN);
}

unsigned char pciide::ide_polling(unsigned char channel, unsigned int advanced_check) {
 
   // (I) Delay 400 nanosecond for BSY to be set:
   // -------------------------------------------------
   for(int i = 0; i < 4; i++)
      ide_read(channel, ATA_REG_ALTSTATUS); // Reading the Alternate Status port wastes 100ns; loop four times.
 
   // (II) Wait for BSY to be cleared:
   // -------------------------------------------------
   while (ide_read(channel, ATA_REG_STATUS) & ATA_SR_BSY)
      ; // Wait for BSY to be zero.
 
   if (advanced_check) {
      unsigned char state = ide_read(channel, ATA_REG_STATUS); // Read Status Register.
 
      // (III) Check For Errors:
      // -------------------------------------------------
      if (state & ATA_SR_ERR)
         return 2; // Error.
 
      // (IV) Check If Device fault:
      // -------------------------------------------------
      if (state & ATA_SR_DF)
         return 1; // Device Fault.
 
      // (V) Check DRQ:
      // -------------------------------------------------
      // BSY = 0; DF = 0; ERR = 0 so we should check for DRQ now.
      if ((state & ATA_SR_DRQ) == 0)
         return 3; // DRQ should be set
 
   }
 
   return 0; // No Error.
 
}

unsigned char pciide::ide_print_error(unsigned int drive, unsigned char err) {
   if (err == 0)
      return err;
 
   printf("IDE:");
   if (err == 1) {printf("- Device Fault\n     "); err = 19;}
   else if (err == 2) {
      unsigned char st = ide_read(ide_devices[drive].Channel, ATA_REG_ERROR);
      if (st & ATA_ER_AMNF)   {printf("- No Address Mark Found\n     ");   err = 7;}
      if (st & ATA_ER_TK0NF)   {printf("- No Media or Media Error\n     ");   err = 3;}
      if (st & ATA_ER_ABRT)   {printf("- Command Aborted\n     ");      err = 20;}
      if (st & ATA_ER_MCR)   {printf("- No Media or Media Error\n     ");   err = 3;}
      if (st & ATA_ER_IDNF)   {printf("- ID mark not Found\n     ");      err = 21;}
      if (st & ATA_ER_MC)   {printf("- No Media or Media Error\n     ");   err = 3;}
      if (st & ATA_ER_UNC)   {printf("- Uncorrectable Data Error\n     ");   err = 22;}
      if (st & ATA_ER_BBK)   {printf("- Bad Sectors\n     ");       err = 13;}
   } else  if (err == 3)           {printf("- Reads Nothing\n     "); err = 23;}
     else  if (err == 4)  {printf("- Write Protected\n     "); err = 8;}
   printf("- [%s %s] %s\n",
      (const char *[]){"Primary", "Secondary"}[ide_devices[drive].Channel], // Use the channel as an index into the array
      (const char *[]){"Master", "Slave"}[ide_devices[drive].Drive], // Same as above, using the drive
      ide_devices[drive].Model);
 
   return err;
}



pciide::pciide(unsigned int BAR0, unsigned int BAR1, unsigned int BAR2, unsigned int BAR3,
unsigned int BAR4){
 
   int i,j, k, count = 0;
 
   // 1- Detect I/O Ports which interface IDE Controller:
   channels[ATA_PRIMARY  ].base  = (BAR0 & 0xFFFFFFFC) + 0x1F0 * (!BAR0);
   channels[ATA_PRIMARY  ].ctrl  = (BAR1 & 0xFFFFFFFC) + 0x3F6 * (!BAR1);
   channels[ATA_SECONDARY].base  = (BAR2 & 0xFFFFFFFC) + 0x170 * (!BAR2);
   channels[ATA_SECONDARY].ctrl  = (BAR3 & 0xFFFFFFFC) + 0x376 * (!BAR3);
   channels[ATA_PRIMARY  ].bmide = (BAR4 & 0xFFFFFFFC) + 0; // Bus Master IDE
   channels[ATA_SECONDARY].bmide = (BAR4 & 0xFFFFFFFC) + 8; // Bus Master IDE

      // 2- Disable IRQs:
   ide_write(ATA_PRIMARY  , ATA_REG_CONTROL, 2);
   ide_write(ATA_SECONDARY, ATA_REG_CONTROL, 2);

   // 3- Detect ATA-ATAPI Devices:
   for (i = 0; i < 2; i++)
      for (j = 0; j < 2; j++) {
 
         unsigned char err = 0, type = IDE_ATA, status;
         ide_devices[count].Reserved = 0; // Assuming that no drive here.
 
         // (I) Select Drive:
         ide_write(i, ATA_REG_HDDEVSEL, 0xA0 | (j << 4)); // Select Drive.
         PIT::Sleep(1); // Wait 1ms for drive select to work.
 
         // (II) Send ATA Identify Command:
         ide_write(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
         PIT::Sleep(1); // This function should be implemented in your OS. which waits for 1 ms.
                   // it is based on System Timer Device Driver.
 
         // (III) Polling:
         if (ide_read(i, ATA_REG_STATUS) == 0) continue; // If Status = 0, No Device.
 
         while(1) {
            status = ide_read(i, ATA_REG_STATUS);
            if ((status & ATA_SR_ERR)) {err = 1; break;} // If Err, Device is not ATA.
            if (!(status & ATA_SR_BSY) && (status & ATA_SR_DRQ)) break; // Everything is right.
         }
 
         // (IV) Probe for ATAPI Devices:
 
         if (err != 0) {
            unsigned char cl = ide_read(i, ATA_REG_LBA1);
            unsigned char ch = ide_read(i, ATA_REG_LBA2);
 
            if (cl == 0x14 && ch ==0xEB)
               type = IDE_ATAPI;
            else if (cl == 0x69 && ch == 0x96)
               type = IDE_ATAPI;
            else
               continue; // Unknown Type (may not be a device).
 
            ide_write(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY_PACKET);
            PIT::Sleep(1);
         }
 
        // (V) Read Identification Space of the Device:
        for(uint_16 k = 0;k < 256;k++){
            uint_16 data = ide_read(i,ATA_REG_DATA);
            ((uint_16*)ide_buf)[k] = data;
        }
 
         // (VI) Read Device Parameters:
         ide_devices[count].Reserved     = 1;
         ide_devices[count].Type         = type;
         ide_devices[count].Channel      = i;
         ide_devices[count].Drive        = j;
         ide_devices[count].Signature    = *((unsigned short *)(ide_buf + ATA_IDENT_DEVICETYPE));
         ide_devices[count].Capabilities = *((unsigned short *)(ide_buf + ATA_IDENT_CAPABILITIES));
         ide_devices[count].CommandSets  = *((unsigned int *)(ide_buf + ATA_IDENT_COMMANDSETS));
 
         // (VII) Get Size:
         if (ide_devices[count].CommandSets & (1 << 26))
            // Device uses 48-Bit Addressing:
            ide_devices[count].Size   = *((unsigned int *)(ide_buf + ATA_IDENT_MAX_LBA_EXT));
         else
            // Device uses CHS or 28-bit Addressing:
            ide_devices[count].Size   = *((unsigned int *)(ide_buf + ATA_IDENT_MAX_LBA));
 
         // (VIII) String indicates model of device (like Western Digital HDD and SONY DVD-RW...):
         for(k = 0; k < 40; k += 2) {
            ide_devices[count].Model[k] = ide_buf[ATA_IDENT_MODEL + k + 1];
            ide_devices[count].Model[k + 1] = ide_buf[ATA_IDENT_MODEL + k];}
         ide_devices[count].Model[40] = 0; // Terminate String.
 
         count++;
      }
 
   // 4- Print Summary:
   for (i = 0; i < 4 ; i++)
      if (ide_devices[i].Reserved == 1) {
         printf(" Found %s Drive %dB - %s\n",
            (const char *[]){"ATA", "ATAPI"}[ide_devices[i].Type],         /* Type */
            ide_devices[i].Size,               /* Size */
            ide_devices[i].Model);
            ATADevice dev;
            dev.parent = this;
            dev.index_inside_parent = i;
            ATA::devs[devs_size] = dev;
            devs_size++;
      }
}

unsigned char pciide::ide_ata_access(unsigned char direction, unsigned char drive, unsigned int lba, 
                             unsigned char numsects,unsigned char* destination) {

    unsigned char lba_mode /* 0: CHS, 1:LBA28, 2: LBA48 */, dma /* 0: No DMA, 1: DMA */, cmd;
   unsigned char lba_io[6];
   unsigned int  channel      = ide_devices[drive].Channel; // Read the Channel.
   unsigned int  slavebit      = ide_devices[drive].Drive; // Read the Drive [Master/Slave]
   unsigned int  bus = channels[channel].base; // Bus Base, like 0x1F0 which is also data port.
   unsigned int  words      = 256; // Almost every ATA drive has a sector-size of 512-byte.
   unsigned short cyl, i;
   unsigned char head, sect, err;

   ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN = 0x02);

    // (I) Select one from LBA28, LBA48 or CHS;
   if (lba >= 0x10000000) { // Sure Drive should support LBA in this case, or you are
                            // giving a wrong LBA.
      // LBA48:
      lba_mode  = 2;
      lba_io[0] = (lba & 0x000000FF) >> 0;
      lba_io[1] = (lba & 0x0000FF00) >> 8;
      lba_io[2] = (lba & 0x00FF0000) >> 16;
      lba_io[3] = (lba & 0xFF000000) >> 24;
      lba_io[4] = 0; // LBA28 is integer, so 32-bits are enough to access 2TB.
      lba_io[5] = 0; // LBA28 is integer, so 32-bits are enough to access 2TB.
      head      = 0; // Lower 4-bits of HDDEVSEL are not used here.
   } else if (ide_devices[drive].Capabilities & 0b0000000000000000)  { // Drive supports LBA?
      // LBA28:
      lba_mode  = 1;
      lba_io[0] = (lba & 0x00000FF) >> 0;
      lba_io[1] = (lba & 0x000FF00) >> 8;
      lba_io[2] = (lba & 0x0FF0000) >> 16;
      lba_io[3] = 0; // These Registers are not used here.
      lba_io[4] = 0; // These Registers are not used here.
      lba_io[5] = 0; // These Registers are not used here.
      head      = (lba & 0xF000000) >> 24;
   } else {
      // CHS:
      lba_mode  = 0;
      sect      = (lba % 63) + 1;
      cyl       = (lba + 1  - sect) / (16 * 63);
      lba_io[0] = sect;
      lba_io[1] = (cyl >> 0) & 0xFF;
      lba_io[2] = (cyl >> 8) & 0xFF;
      lba_io[3] = 0;
      lba_io[4] = 0;
      lba_io[5] = 0;
      head      = (lba + 1  - sect) % (16 * 63) / (63); // Head number is written to HDDEVSEL lower 4-bits.
   }

    // (II) See if drive supports DMA or not;
   dma = 0; // We don't support DMA

      // (III) Wait if the drive is busy;
   while (ide_read(channel, ATA_REG_STATUS) & ATA_SR_BSY){}

    // (IV) Select Drive from the controller;
    if (lba_mode == 0)
       ide_write(channel, ATA_REG_HDDEVSEL, 0xA0 | (slavebit << 4) | head); // Drive & CHS.
    else
       ide_write(channel, ATA_REG_HDDEVSEL, 0xE0 | (slavebit << 4) | head); // Drive & LBA
      printf("s : %i",lba_mode);
    // (V) Write Parameters;
    if (lba_mode == 2) {
       ide_write(channel, ATA_REG_SECCOUNT1,   0);
       ide_write(channel, ATA_REG_LBA3,   lba_io[3]);
       ide_write(channel, ATA_REG_LBA4,   lba_io[4]);
       ide_write(channel, ATA_REG_LBA5,   lba_io[5]);
    }
    ide_write(channel, ATA_REG_SECCOUNT0,   numsects);
    ide_write(channel, ATA_REG_LBA0,   lba_io[0]);
    ide_write(channel, ATA_REG_LBA1,   lba_io[1]);
    ide_write(channel, ATA_REG_LBA2,   lba_io[2]);

    if (lba_mode == 0 && dma == 0 && direction == 0) cmd = ATA_CMD_READ_PIO;
    if (lba_mode == 1 && dma == 0 && direction == 0) cmd = ATA_CMD_READ_PIO;   
    if (lba_mode == 2 && dma == 0 && direction == 0) cmd = ATA_CMD_READ_PIO_EXT;   
    if (lba_mode == 0 && dma == 1 && direction == 0) cmd = ATA_CMD_READ_DMA;
    if (lba_mode == 1 && dma == 1 && direction == 0) cmd = ATA_CMD_READ_DMA;
    if (lba_mode == 2 && dma == 1 && direction == 0) cmd = ATA_CMD_READ_DMA_EXT;
    if (lba_mode == 0 && dma == 0 && direction == 1) cmd = ATA_CMD_WRITE_PIO;
    if (lba_mode == 1 && dma == 0 && direction == 1) cmd = ATA_CMD_WRITE_PIO;
    if (lba_mode == 2 && dma == 0 && direction == 1) cmd = ATA_CMD_WRITE_PIO_EXT;
    if (lba_mode == 0 && dma == 1 && direction == 1) cmd = ATA_CMD_WRITE_DMA;
    if (lba_mode == 1 && dma == 1 && direction == 1) cmd = ATA_CMD_WRITE_DMA;
    if (lba_mode == 2 && dma == 1 && direction == 1) cmd = ATA_CMD_WRITE_DMA_EXT;
    ide_write(channel, ATA_REG_COMMAND, cmd);   

    if (dma)
      if (direction == 0);
         // DMA Read.
      else;
         // DMA Write.
    else{
        if (direction == 0)
        // PIO Read.
        for (i = 0; i < numsects; i++) {
            if (err = ide_polling(channel, 1))
               return err; // Polling, set error and exit if there is.
            for (size_t j = 0; j < words; j++)
            {
               ((uint_16*)destination)[j] = ide_read(bus,ATA_REG_DATA);
            }
        } else {
        // PIO Write.
           for (i = 0; i < numsects; i++) {
            ide_polling(channel, 0); // Polling.
            for (size_t j = 0; j < words; j++)
            {
               ide_write(bus,ATA_REG_DATA,((uint_16*)destination)[j]);
            }
            
        }
        ide_write(channel, ATA_REG_COMMAND, (char []) {(char)ATA_CMD_CACHE_FLUSH,
                        (char)ATA_CMD_CACHE_FLUSH,
                        (char)ATA_CMD_CACHE_FLUSH_EXT}[lba_mode]);
        ide_polling(channel, 0); // Polling.
        }
    }
   return 0;
}
void pciide::ide_read_slash_write(int device,char direction,uint_8* destination,int address,int sector_count) {
 
   
   // 1: Check if the drive presents:
   // ==================================
   if (device > 3 || ide_devices[device].Reserved == 0) return;      // Drive Not Found!
 
   // 2: Check if inputs are valid:
   // ==================================
   else if (((address + sector_count) > ide_devices[device].Size) && (ide_devices[device].Type == IDE_ATA))
      return;                     // Seeking to invalid position.
 
   // 3: Read in PIO Mode through Polling & IRQs:
   // ============================================
   else {
      unsigned char err;
      if (ide_devices[device].Type == IDE_ATA)
         err = ide_ata_access(direction, device, address, sector_count, (unsigned char*)destination);
      else if (ide_devices[device].Type == IDE_ATAPI)
            printf("ERROR: ATAPI not supported\n");
      ide_print_error(device, err);
   }
}
