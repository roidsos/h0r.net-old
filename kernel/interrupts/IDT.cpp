#include <interrupts/IDT.h>

IDT::Interrupt IDT::Entries[256];
uint_64 IDT::HandleInterrupt(uint_8 number ,uint_64 rsp)
{
    print("interrupt!!");
    return rsp;
}

/*
PICMasterCommandPort=0x20
PICMasterDataPort=0x21
PICSlaveCommandPort=0xA0
PICSlaveDataPort=0xA1
*/

IDT::IDT(GDT* gdt)
{

    //get some data
    uint_16 code_offset = gdt->GetCodeseg();
    const uint_8 IDT_INTERRUPT_GATE = 0xe;
    //fill the IDT with "air"
    for(uint_16 i = 0;i < 256;i++)
        SetIDTEntry(i,code_offset,&InterruptIgnore,0,IDT_INTERRUPT_GATE);
    //add interrupts to the IDT
    SetIDTEntry(0x20,code_offset,&HandleInterruptNumber0x00,0,IDT_INTERRUPT_GATE);//timer interrupt
    SetIDTEntry(0x21,code_offset,&HandleInterruptNumber0x01,0,IDT_INTERRUPT_GATE);//keyboard interrupt
    //command the PICs
    outbslow8(0x20,0x11);
    outbslow8(0xA0,0x11);
    // give the irq_base to the PICs
    outbslow8(0x21,0x20);
    outbslow8(0xA1,0x28);
    //tell the PICs their role
    outbslow8(0x21,0x04);
    outbslow8(0xA1,0x02);
    //give the PICs more info
    outbslow8(0x21,0x01);
    outbslow8(0xA1,0x01);

    outbslow8(0x21,0x00);
    outbslow8(0xA1,0x00);

    IDT_pointer idt;
    idt.size = 256 * sizeof(Interrupt) - 1;
    idt.base = (uint_32)Entries;
    asm volatile ("lidt %0": : "m" (idt));
}

IDT::~IDT()
{
    
}



static void IDT::Activate(){

    asm("sti");
}

void IDT::SetIDTEntry(uint_8 num,
        uint_16 gdt_offset_code,
        void (*handler)(),
        uint_8 access,
        uint_8 type
){
Entries[num].handler_low     = (((uint_32)handler)) & 0xffff;
Entries[num].handler_hi      = (((uint_32)handler) >> 16) & 0xffff;
Entries[num].gdt_offset_code = gdt_offset_code;
Entries[num].access          = 0x80 | type | ((access&3) << 5);
Entries[num].reserved        = 0;
}