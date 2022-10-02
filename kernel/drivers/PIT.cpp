#include "PIT.h"
#include "io/io.h"


namespace PIT
{
    volatile int UptmeInMillis;
    volatile int countdown;
    void PitInit()
    {
        // this code was yoinked from https://github.com/pdoane/osdev/blob/master/time/pit.c and we totally read the license!!!!!
        // uint_32 PIT_FREQUENCY  = 1193182;
        uint_32 hz = 1000;
        uint_32 divisor = PIT_FREQUENCY / hz;
        outb8(PIT_CMD, CMD_BINARY | CMD_MODE3 | CMD_RW_BOTH | CMD_COUNTER0);
        outb8(PIT_COUNTER0, divisor);
        outb8(PIT_COUNTER0, divisor >> 8);
    }

    void Sleep(uint_32 millis) 
    {
        countdown =  millis;

        while (countdown != 0)
        {}
    }
}

extern "C" void Tick()
{
    PIT::UptmeInMillis++;
    
    if (PIT::countdown != 0)
    {
        PIT::countdown--;
    }

    outb8(0x20,0x20);
    outb8(0xa0,0x20);
}