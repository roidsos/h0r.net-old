#include "PIT.h"
#include "io.h"



namespace PIT{
    double TimeSinceBoot = 0;

    uint_16 Divisor = 65535;

    void Sleepd(double seconds){
        double startTime = TimeSinceBoot;
        while (TimeSinceBoot < startTime + seconds){
            asm("hlt");
        }
    }

    void Sleep(uint_64 milliseconds){
        Sleepd((double)milliseconds / 1000);
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
        PIT::TimeSinceBoot += 1 / (double)PIT::GetFrequency();
    }