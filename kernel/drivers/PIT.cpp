#include "PIT.h"
#include "io.h"



namespace PIT{
    double TimeSinceBoot = 0;

    uint_16 Divisor = 65535;

    void Sleepd(double seconds){
    double startTime = TimeSinceBoot;
        while (TimeSinceBoot <= startTime + seconds){
            asm volatile("nop");
        }
    }

    void Sleep(uint_64 milliseconds){
        Sleepd((double)milliseconds / 1000);
    }

    void SetDivisor(uint_16 divisor){
        if (divisor < 100) divisor = 100;
        Divisor = divisor;
        outb8(0x40, (uint_8)(divisor & 0x00ff));
        IOWait();
        outb8(0x40, (uint_8)((divisor & 0xff00) >> 8));
    }

    uint_64 GetFrequency(){
        return BaseFrequency / Divisor;
    }

    void SetFrequency(uint_64 frequency){
        SetDivisor(BaseFrequency / frequency);
    }

}
    extern "C" void Tick(){
        PIT::TimeSinceBoot += 1 / (double)PIT::GetFrequency();
        outb8(0x20,0x20);
        outb8(0xa0,0x20);
    }