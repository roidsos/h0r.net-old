
#pragma once
#include <stdint.h>

namespace PIT {
    extern double TimeSinceBoot;
    const uint_64 BaseFrequency = 1193182;

    void Sleepd(double Seconds);
    void Sleep(uint_64 MilliSeconds);

    uint_64 GetFrequency();
    void SetDivisor(uint_16 Divisor);
    void SetFrequency(uint_64 Frequency);
}