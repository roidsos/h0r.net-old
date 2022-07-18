#include "PIT.h"
#include "io.h"



namespace PIT{
    uint_64 tickssincestart  = 0;
    uint_64 startTime;
    uint_16 Divisor = 65535;

    void Sleep(uint_64 miliseconds){
        startTime = tickssincestart;
        while (tickssincestart <= startTime + miliseconds){}
    }


    void SetDivisor(uint_64 Divisor){
        if (Divisor < 100) Divisor = 100;
        Divisor = Divisor;
        outb8(0x40, (uint_8)(Divisor & 0x00ff));
        IOWait();
        outb8(0x40, (uint_8)((Divisor & 0xff00) >> 8));
    }

    uint_64 GetFrequency(){
        return BaseFrequency / Divisor;
    }

    void SetFrequency(uint_64 frequency){
        SetDivisor(BaseFrequency / frequency);
    }


}
    extern "C" void Tick(){
        PIT::tickssincestart += 1;
        outb8(0x20,0x20);
        outb8(0xa0,0x20);
    }